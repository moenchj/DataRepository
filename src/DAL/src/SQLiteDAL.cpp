#include "SQLiteDAL.h"

#include <stdexcept>
#include <functional>
#include <mutex>
#include <iostream>
#include <variant>
#include <sstream>
#include <tuple>

using namespace std;

void checkAndHandleSQLiteError(const int rc, char* errorMsg = nullptr)
{
    if(rc != SQLITE_OK)
    {
        string errorMessageString;
        if(errorMsg)
        {
            errorMessageString = errorMsg;
            sqlite3_free(errorMsg);
        }
        runtime_error exceptionObj(string("Runtime error in SQLiteDAL. ") + errorMessageString);
        throw exceptionObj;
    }
}

namespace DAL
{
    recursive_mutex SQLiteDAL::sqLiteMutex;

    class Binder
    {
        private:
            sqlite3_stmt* compiledStatement;
            int index;

        public:
            Binder(sqlite3_stmt* compiledStatement, const int index)
            : compiledStatement(compiledStatement), index(index) {}

            void operator()(const double value)
            { 
                int rc = sqlite3_bind_double(compiledStatement, index, value);
                checkAndHandleSQLiteError(rc);                
            }

            void operator()(const long long value)
            { 
                int rc = sqlite3_bind_int64(compiledStatement, index, value);
                checkAndHandleSQLiteError(rc);
            }

            void operator()(const string& value)
            { 
                int rc = sqlite3_bind_text(compiledStatement, index, value.c_str(), -1, SQLITE_TRANSIENT);
                checkAndHandleSQLiteError(rc);
            }

            void operator()(const monostate& value)
            { 
                int rc = sqlite3_bind_null(compiledStatement, index);
                checkAndHandleSQLiteError(rc);
            }
    };

    SQLiteDAL::~SQLiteDAL()
    {
        sqlite3_close(_db);
    }

    SQLiteDAL::SQLiteDAL(const string& dbFilePath)
    : _lastInsertQuery(DBQuery("select last_insert_rowid()", 1))
    {
        int rc = sqlite3_open(dbFilePath.c_str(), &_db);
        checkAndHandleSQLiteError(rc);
    }

    int SQLiteDAL::processRowData(void* pObject, int numColumns, char** columnData, char** columnNames)
    {
        SQLiteDAL* pSQLiteDAL = static_cast<SQLiteDAL*>(pObject);
        DBRow row(numColumns);
        for(int i = 0; i < numColumns; i++)
        {
            row.headings[i] = columnNames[i];
            row.values[i] = columnData[i];
        }
        pSQLiteDAL->_currentRowList.push_back(row);
        return 0;
    }

    list<DBRow> SQLiteDAL::sqlQuery(const string& query)
    {
        list<DBRow> rowList;
        char* errorMsg = nullptr;
        int rc = sqlite3_exec(_db, query.c_str(), &SQLiteDAL::processRowData, this, &errorMsg);
        checkAndHandleSQLiteError(rc, errorMsg);
        rowList.splice(rowList.begin(), _currentRowList);
        return rowList;
    }

    void SQLiteDAL::bindValues(const DBQuery& query, const size_t startAt)
    {
        sqlite3_stmt* compiledStatement = static_cast<sqlite3_stmt*>(query.PreparedDALRef());
        auto boundValues = query.BoundValues();
        if(sqlite3_bind_parameter_count(compiledStatement) != (boundValues.size() + 2))
        {
            throw runtime_error(string("Runtime error in SQLiteDAL. Number of parameters given does not match query. \"") + 
                query.Query() + "\"");
        }
        int i = 1;
        for(auto& item: boundValues)
        {
            visit(Binder {compiledStatement, i++}, item);
        }
        visit(Binder {compiledStatement, i++}, DBValue(static_cast<long long>(query.PageSize())));
        visit(Binder {compiledStatement, i++}, DBValue(static_cast<long long>(startAt)));
    }

    list<DBRow> SQLiteDAL::sqlQuery(const DBQuery& query, const std::size_t startAt)
    {
        bindValues(query, startAt);

        list<DBRow> rowList;
        sqlite3_stmt* compiledStatement = static_cast<sqlite3_stmt*>(query.PreparedDALRef());
        int numColumns = sqlite3_column_count(compiledStatement);
        vector<string> columnNames(numColumns);
        for(int col = 0; col < numColumns; ++col)
        {
            columnNames[col] = sqlite3_column_name(compiledStatement, col);
        }

        int rc = 0;
        while((rc = sqlite3_step(compiledStatement)) == SQLITE_ROW)
        {
            DBRow row {numColumns};            
            for(int col = 0; col < numColumns; ++col)
            {
                row.headings[col] = columnNames[col];
                sqlite3_value* columnValue = sqlite3_column_value(compiledStatement, col);
                sqlite3_value* dupColValue = sqlite3_value_dup(columnValue);
                int dataType = sqlite3_value_type(dupColValue);
                if(dataType == SQLITE_INTEGER)
                {
                    row.values[col] = sqlite3_value_int64(dupColValue);
                }
                else if(dataType == SQLITE_TEXT)
                {
                    row.values[col] = reinterpret_cast<const char*>(sqlite3_value_text(dupColValue));
                }
                else if(dataType == SQLITE_BLOB)
                {
                    cout << "Blob value ignored" << endl;
                }
                else if(dataType == SQLITE_NULL)
                {
                    row.values[col] = monostate();
                }
                sqlite3_value_free(dupColValue);
            }
            rowList.push_back(row);
        }
        sqlite3_reset(compiledStatement);
        return rowList;
    }

    void SQLiteDAL::compileQuery(const DBQuery& query)
    {
        sqlite3_stmt* compiledQuery;
        stringstream sqliteQuery;
        sqliteQuery << query.Query() << " LIMIT ? OFFSET ?";
        int rc = sqlite3_prepare_v2(_db, sqliteQuery.str().c_str(), -1, &compiledQuery, NULL);
        checkAndHandleSQLiteError(rc);
        query.PreparedDalRef(compiledQuery);
    }

    list<DBRow> SQLiteDAL::selectQuery(const DBQuery& query, const std::size_t startAt)
    {        
        scoped_lock lock(sqLiteMutex);
        if(!query.PreparedDALRef())
        {
            compileQuery(query);
        }        
        return sqlQuery(query, startAt);
    }

    DBInsertResult SQLiteDAL::insertQuery(const string& table, const vector<DBInsertValue>& values)
    {
        scoped_lock lock(sqLiteMutex);
        stringstream query;
        query << "insert into " << table << " values (";
        string currentQuery = query.str();
        bool separatorNeeded = false;
        bool hasAutoValue = false;
        for(auto value: values)
        {
            if(separatorNeeded)
            {
                query << ", ";
                separatorNeeded = false;
            }
            if(holds_alternative<DBAutoValue>(value))
            {
                hasAutoValue = true;
                query << "null";
            }
            else
            {
                DBValue dbValue = get<DBValue>(value);
                if(holds_alternative<string>(dbValue))
                {
                    query << "\"" << get<string>(dbValue) << "\"";
                }
                else
                {
                    query << get<long long>(dbValue);
                }          
            }
            separatorNeeded = true;        
            currentQuery = query.str();
        }
        query << ")";

        sqlQuery(query.str());

        if(hasAutoValue)
        {
            list<DBRow> result = selectQuery(_lastInsertQuery);                
            DBValue resultValue = result.front().values[0];
            if(auto value = get_if<long long>(&resultValue))
            {
                return variant<monostate, long long>(*value);
            }
        }
        return variant<monostate, long long>();
    }

    void SQLiteDAL::updateQuery(const string& table, const map<string, DBValue>& valueMap, const string& whereClause)
    {
        scoped_lock lock(sqLiteMutex);
        stringstream query;
        query << "update " << table << " set ";
        bool separatorNeeded = false;
        for(auto valueTuple: valueMap)
        {
            if(separatorNeeded)
            {
                query << ", ";
                separatorNeeded = false;
            }
            query << get<0>(valueTuple) << "=" ;
            DBValue dbValue = get<1>(valueTuple);
            query << "\"" << get<string>(dbValue) << "\"";
            separatorNeeded = true;
        }
        query << " where " << whereClause;

        sqlQuery(query.str());
    }
};