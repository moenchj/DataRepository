#pragma once

#include <list>
#include <string>

#include "Address.h"
#include "ResultSet.h"
#include "PersistanceService.h"
#include "DAL.h"
#include "DBRow.h"
#include "DBValue.h"

namespace DO
{
    class DataRepository;

    class AddressRepo : public PersistanceService<Address>
    {
        protected:
            DAL::DAL& dal;
            AddressRepo(DAL::DAL& dal) : dal(dal) {}
            std::list<Address> select(const DAL::DBQuery& query, const size_t startAt);
            Address createAddressFromDBRow(const DAL::DBRow& dbRow);

        public:
            Address createAddress();
            void save(Address& address);

            ResultSet<Address> getAddressById(const DAL::DBId id);
            ResultSet<Address> getAddressByStreet(const std::string& street);

            friend DataRepository;
    };
};