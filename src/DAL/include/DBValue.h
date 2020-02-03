#pragma once

#include <variant>
#include <string>

namespace DAL
{
    struct DBAutoValue {};
    using DBValue = std::variant<std::monostate, std::string, long long, double>;
    using DBInsertValue = std::variant<DBAutoValue, DBValue>;
    using DBFieldName = std::string;
    using DBTableName = std::string;
    using DBId = long long;
    using DBInsertResult = std::variant<std::monostate, DBId>;
};