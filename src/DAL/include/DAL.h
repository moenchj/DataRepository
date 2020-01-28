#pragma once

#include <list>
#include <map>
#include <vector>
#include <string>
#include <variant>

#include "DBRow.h"

struct DBAutoValue {};
using DBValue = std::variant<std::monostate, std::string, long long>;
using DBInsertValue = std::variant<DBAutoValue, DBValue>;

class DAL
{
    public:
        virtual std::list<DBRow> selectQuery(const std::string& query, unsigned int startAt = 0, unsigned int numEntries = 100) = 0;
        virtual std::variant<std::monostate, long long> insertQuery(const std::string& table, const std::vector<DBInsertValue>& values) = 0;
        virtual void updateQuery(const std::string& table, const std::map<std::string, DBValue>& valueMap, const std::string& whereClause) = 0;
};