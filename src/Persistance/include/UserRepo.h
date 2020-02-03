#pragma once

#include <list>
#include <string>

#include "User.h"
#include "ResultSet.h"
#include "PersistanceService.h"
#include "DAL.h"
#include "DBRow.h"
#include "DBValue.h"

namespace DO
{
    class DataRepository;

    class UserRepo : public PersistanceService<User>
    {
        protected:
            DAL::DAL& dal;
            UserRepo(DAL::DAL& dal) : dal(dal) {}
            std::list<User> select(const DAL::DBQuery& query, const size_t startAt);
            User createUserFromDBRow(const DAL::DBRow& dbRow);
            Date createDateFromDateField(const std::string& dateFieldValue);
            std::string transformToSQLiteDateString(const Date& date);

        public:
            User createUser();
            void save(User& domainObject);

            ResultSet<User> getUserById(const DAL::DBId id);
            ResultSet<User> getUsersBySurName(const std::string& surname);

            friend DataRepository;
    };
}