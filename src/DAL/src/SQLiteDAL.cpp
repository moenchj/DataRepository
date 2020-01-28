#include <stdexcept>
#include <functional>
#include <mutex>
#include <iostream>
#include <variant>
#include <sstream>
#include <tuple>

#include "SQLiteDAL.h"

using namespace std;

mutex SQLiteDAL::_rowProcessingMutex;

SQLiteDAL::~SQLiteDAL()
{
    sqlite3_close(_db);
}

SQLiteDAL::SQLiteDAL(const string& dbFilePath)
{
    int rc = sqlite3_open(dbFilePath.c_str(), &_db);    
    if(rc != SQLITE_OK)
    {
        throw invalid_argument("Could not open SQLite DB");
    }
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
    scoped_lock lock(_rowProcessingMutex);
    char* errorMsg = nullptr;
    list<DBRow> rowList;
    int rc = sqlite3_exec(_db, query.c_str(), &SQLiteDAL::processRowData, this, &errorMsg);
    if(rc != SQLITE_OK) {
        string errorMessageString = errorMsg;
        runtime_error exceptionObj(errorMsg);
        sqlite3_free(errorMsg);
        throw exceptionObj;
    }
    rowList.splice(rowList.begin(), _currentRowList);
    return rowList;
}

list<DBRow> SQLiteDAL::selectQuery(const string& query, unsigned int startAt, unsigned int numEntries)
{
    stringstream sqliteQuery;
    sqliteQuery << query << " LIMIT " << numEntries << " OFFSET " << startAt;
    string currentQuery = sqliteQuery.str();
    char* errorMsg = nullptr;
    
    return sqlQuery(sqliteQuery.str());
}

variant<monostate, long long> SQLiteDAL::insertQuery(const string& table, const vector<DBInsertValue>& values)
{
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
        list<DBRow> result = selectQuery("select last_insert_rowid()", 0, 1);                
        return variant<monostate, long long>(atoll(result.front().values[0].c_str()));
    }
    return variant<monostate, long long>();
}

void SQLiteDAL::updateQuery(const string& table, const map<string, DBValue>& valueMap, const string& whereClause)
{
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
