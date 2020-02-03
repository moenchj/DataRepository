#pragma once

#include <memory>

namespace DO
{
    template<typename T>
    class PersistanceService;

    template<typename T>
    class BaseDO
    {
        protected:
            long long _id;
            bool dirty;
            PersistanceService<T>* pPersistanceService;

            BaseDO(PersistanceService<T>* pPersistanceService, long long id)
            : pPersistanceService(pPersistanceService), _id(id)
            {}

            void id(long long id)
            {
                _id = id;
            }

            void setDirty()
            {
                dirty = true;
            }

            void save()
            {
                pPersistanceService->save(*this);
                dirty = false;
            }

        public:
            long long id() const
            {
                return _id;
            }

            bool Dirty() const
            {
                return dirty;
            }
    };
};