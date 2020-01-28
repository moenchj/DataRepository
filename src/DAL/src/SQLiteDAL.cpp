#include <stdexcept>
#include <functional>
#include <mutex>
#include <iostream>

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

list<DBRow> SQLiteDAL::executeQuery(const std::string& query, unsigned int startAt, unsigned int numEntries)
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
