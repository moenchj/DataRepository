#pragma once

#include "DAL.h"

class DemoDAL : public DAL
{
    std::list<DBRow> executeQuery(const std::string& query, unsigned int startAt, unsigned int numEntries);
};