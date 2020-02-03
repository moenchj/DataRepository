#pragma once

#include <list>
#include <map>
#include <vector>
#include <string>
#include <variant>

#include "DBValue.h"
#include "DBRow.h"
#include "DBQuery.h"

namespace DAL
{
    class DAL
    {
        public:
            virtual std::list<DBRow> selectQuery(const DBQuery& query, const std::size_t startAt) = 0;
            virtual std::variant<std::monostate, DBId> insertQuery(const DBTableName& table, const std::vector<DBInsertValue>& values) = 0;
            virtual void updateQuery(const DBTableName& table, const std::map<DBFieldName, DBValue>& valueMap, const std::string& whereClause) = 0;
    };
};