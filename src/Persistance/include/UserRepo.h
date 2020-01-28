#pragma once

#include <list>
#include <string>

#include "User.h"
#include "ResultSet.h"
#include "PersistanceService.h"
#include "DAL.h"
#include "DBRow.h"

class DataRepository;

class UserRepo : public PersistanceService<User>
{
    protected:
        DAL& dal;
        UserRepo(DAL& dal) : dal(dal) {}
        std::list<User> select(const std::string& query, unsigned int startAt, unsigned int numEntries);
        User createUserFromDBRow(const DBRow& dbRow);
        Date createDateFromDateField(const std::string& dateFieldValue);
        Address createAddressFromDBRow(const DBRow& dbRow);
        std::string transformToSQLiteDateString(const Date& date);
        long long save(Address& address);

    public:
        User createUser();
        long long save(User& domainObject);

        ResultSet<User> getUserById(const unsigned int id);
        ResultSet<User> getUsersBySurName(const std::string& surname);

        friend DataRepository;
};
