#pragma once

#include <string>
#include <list>

#include "DBQuery.h"

namespace DO
{
    template<typename T>
    class PersistanceService
    {
        protected:
            virtual void save(T& domainObject) = 0;

        public:
            virtual std::list<T> select(const DAL::DBQuery& query, const size_t startAt = 0) = 0;
    };
};