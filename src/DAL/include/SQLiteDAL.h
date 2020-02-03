#pragma once

#include <string>
#include <list>
#include <mutex>
#include <map>
#include <vector>
#include <variant>

#include <sqlite3.h>

#include "DAL.h"

namespace DAL
{
    class SQLiteDAL : public DAL
    {
        private:
            sqlite3* _db;
            std::list<DBRow> _currentRowList;
            DBQuery _lastInsertQuery;
            static std::recursive_mutex sqLiteMutex;

            static int processRowData(void* unused, int numColumns, char** columnData, char** columnNames);
            std::list<DBRow> sqlQuery(const std::string& query);
            std::list<DBRow> sqlQuery(const DBQuery& query, std::size_t startAt = 0);
            void compileQuery(const DBQuery& query);
            void bindValues(const DBQuery& query, const std::size_t startAt);

        public:
            virtual ~SQLiteDAL();
            SQLiteDAL(const std::string& dbFilePath);
            std::list<DBRow> selectQuery(const DBQuery& query, const std::size_t startAt = 0);
            DBInsertResult insertQuery(const DBTableName& table, const std::vector<DBInsertValue>& values);
            void updateQuery(const DBTableName& table, const std::map<DBFieldName, DBValue>& valueMap, const std::string& whereClause);
    };
};