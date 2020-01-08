#pragma once

#include <string>
#include <list>

template<typename T>
class PersistanceService
{
    public:
        virtual std::list<T> executeQuery(const std::string& query, unsigned int startAt = 0, unsigned int numEntries = 100) = 0;
        virtual bool save(T& domainObject) = 0;
};
