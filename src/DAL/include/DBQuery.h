#pragma once

#include <string>
#include <utility>
#include <functional>
#include <variant>
#include <list>
#include <exception>

#include "DBValue.h"

namespace DAL
{
    class DBQuery
    {
        private:
            std::string _query;
            std::list<DBValue> _boundValues;
            std::size_t _pageSize;

            mutable void* _pPreparedDALRef;

            template <typename T>
            void consumeBindValue(T arg)
            {
                _boundValues.push_back(arg);
            }

            void _bindValues() {}

            template <typename T, typename ... Types>
            void _bindValues(T arg0, Types ... args)
            {
                consumeBindValue(arg0);
                _bindValues(args...);
            }

        public:
            DBQuery(const std::string& query, const std::size_t pageSize = 100) 
            : _query(query), _pPreparedDALRef(nullptr), _pageSize(pageSize)
            {
                if(pageSize == 0)
                {
                    throw std::runtime_error(std::string("Runtime error in DBQUery. Page size must not be Zero!"));
                }
            }

            template <typename ... Types>
            void bindValues(Types ... args)
            {
                _boundValues.clear();
                _bindValues(args...);
            }

            const std::string& Query() const
            {
                return _query;
            }

            const std::list<DBValue>& BoundValues() const
            {
                return _boundValues;
            }

            void* PreparedDALRef() const
            {
                return _pPreparedDALRef;
            }

            void PreparedDalRef(void* pPreparedDALRef) const
            {
                _pPreparedDALRef = pPreparedDALRef;
            }

            std::size_t PageSize() const
            {
                return _pageSize;
            }
    };
};