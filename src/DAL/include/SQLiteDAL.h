#pragma once

#include <string>
#include <list>
#include <mutex>
#include <map>
#include <vector>
#include <variant>

#include <sqlite3.h>

#include "DAL.h"

class SQLiteDAL : public DAL
{
    private:
        sqlite3* _db;
        std::list<DBRow> _currentRowList;
        static std::mutex _rowProcessingMutex;

        static int processRowData(void* unused, int numColumns, char** columnData, char** columnNames);
        std::list<DBRow> sqlQuery(const std::string& query);
    public:
        virtual ~SQLiteDAL();
        SQLiteDAL(const std::string& dbFilePath);
        std::list<DBRow> selectQuery(const std::string& query, unsigned int startAt, unsigned int numEntries);
        std::variant<std::monostate, long long> insertQuery(const std::string& table, const std::vector<DBInsertValue>& values);
        void updateQuery(const std::string& table, const std::map<std::string, DBValue>& valueMap, const std::string& whereClause);
};