#pragma once

#include <vector>
#include <string>

#include "DBValue.h"

namespace DAL
{
    class DBRow
    {
        public:
            std::vector<std::string> headings;
            std::vector<DBValue> values;

            DBRow(int numColumns)
            : headings(numColumns), values(numColumns)
            {}
    };
};