#pragma once

#include <string>
#include <list>
#include <mutex>

#include <sqlite3.h>

#include "DAL.h"

class SQLiteDAL : public DAL
{
    private:
        sqlite3* _db;
        std::list<DBRow> _currentRowList;
        static std::mutex _rowProcessingMutex;

        static int processRowData(void* unused, int numColumns, char** columnData, char** columnNames);

    public:
        virtual ~SQLiteDAL();
        SQLiteDAL(const std::string& dbFilePath);
        std::list<DBRow> executeQuery(const std::string& query, unsigned int startAt, unsigned int numEntries);
};