#pragma once

#include <list>
#include <string>

#include "DBRow.h"

class DAL
{
    virtual std::list<DBRow> executeQuery(const std::string& query, unsigned int startAt = 0, unsigned int numEntries = 100) = 0;
};