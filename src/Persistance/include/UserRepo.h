#pragma once

#include <list>
#include <string>

#include "User.h"
#include "ResultSet.h"
#include "PersistanceService.h"
#include "DAL.h"

class DataRepository;

class UserRepo : public PersistanceService<User>
{
    protected:
        DAL& dal;
        UserRepo(DAL& dal) : dal(dal) {}
        std::list<User> executeQuery(const std::string& query, unsigned int startAt, unsigned int numEntries);

    public:
        bool save(User& domainObject);

        ResultSet<User> getUserById(const unsigned int id);
        ResultSet<User> getUsersBySurName(const std::string& surname);

        friend DataRepository;
};
