#pragma once

#include <memory>

#include "UserRepo.h"
#include "AddressRepo.h"
#include "DAL.h"

namespace DO
{
    class DataRepository
    {
        private:
            static std::unique_ptr<DAL::DAL> _dal;
            static std::unique_ptr<DataRepository> _instance;
            UserRepo _users;
            AddressRepo _addresses;
            DataRepository(DAL::DAL& dal);

        public:
            static DataRepository instance();
            static DataRepository instance(DAL::DAL& dal);
            static void dispose();
            
            UserRepo Users();
            AddressRepo Addresses();
    };
};