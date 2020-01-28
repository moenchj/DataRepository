#pragma once

#include <string>
#include <list>

template<typename T>
class PersistanceService
{
    public:
        virtual std::list<T> select(const std::string& query, unsigned int startAt = 0, unsigned int numEntries = 100) = 0;
        virtual long long save(T& domainObject) = 0;
};
