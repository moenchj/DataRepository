#pragma once

#include <memory>

#include "UserRepo.h"
#include "DAL.h"

class DataRepository
{
    private:
        static std::unique_ptr<DAL> _dal;
        static std::unique_ptr<DataRepository> _instance;
        UserRepo _users;
        DataRepository(DAL& dal);

    public:
        static DataRepository instance();
        static DataRepository instance(DAL& dal);
        static void dispose();
        
        UserRepo Users();
};