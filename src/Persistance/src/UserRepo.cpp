#include "UserRepo.h"

#include <sstream>
#include <string>
#include <cstdlib>
#include <regex>
#include <functional>
#include <iostream>
#include <cstdlib>

#include "PersistanceService.h"
#include "ResultSet.h"
#include "AddressRepo.h"
#include "DataRepository.h"
#include "DBQuery.h"

using namespace std;
using namespace DAL;

namespace DO
{
    User UserRepo::createUser()
    {
        Address homeAddress = DataRepository::instance().Addresses().createAddress();
        return User{this, -1ll, "", "", "", Date{1, 1, 1970}, homeAddress};
    }

    Date UserRepo::createDateFromDateField(const string& dateFieldValue)
    {
        regex dateRegex {"([0-9][0-9][0-9][0-9])-([0-9]+)-([0-9]+)"};
        smatch m;
        if(regex_match(dateFieldValue, m, dateRegex))
        {
            Date date { atoi(m.str(3).c_str()), atoi(m.str(2).c_str()), atoi(m.str(1).c_str()) };
            return date;
        }
        return Date { 0, 0, 0 };
    }

    User UserRepo::createUserFromDBRow(const DBRow& dbRow)
    {
        Date birthDate = createDateFromDateField(get<string>(dbRow.values[4]));
        AddressRepo addresses = DataRepository::instance().Addresses();
        auto addressResult = addresses.getAddressById(get<long long>(dbRow.values[5]));
        if(addressResult.size() > 0) 
        {
            return User { this,
                          get<long long>(dbRow.values[0]), 
                          get<string>(dbRow.values[1]), 
                          get<string>(dbRow.values[2]), 
                          get<string>(dbRow.values[3]), 
                          birthDate,
                          addressResult.front() };
        }
        else
        {
            return User { this,
                           get<long long>(dbRow.values[0]), 
                           get<string>(dbRow.values[1]), 
                           get<string>(dbRow.values[2]), 
                           get<string>(dbRow.values[3]), 
                           birthDate,
                           DataRepository::instance().Addresses().createAddress()};
        }
    }

    list<User> UserRepo::select(const DBQuery& query, const size_t startAt)
    {
        list<User> resultList;
        list<DBRow> dbList = dal.selectQuery(query, startAt);
        transform (dbList.begin(), dbList.end(), back_inserter(resultList), bind(mem_fn(&UserRepo::createUserFromDBRow), this, placeholders::_1));
        return resultList;
    }

    string UserRepo::transformToSQLiteDateString(const Date& date)
    {
        stringstream dateString;
        dateString << date.year() << "-" << date.month() << "-" << date.day();
        return dateString.str();
    }

    void UserRepo::save(User& user)
    {
        AddressRepo addresses = DataRepository::instance().Addresses();
        stringstream query;

        addresses.save(user._homeAddress);
        if(user.id() == -1)
        {   
            vector<DBInsertValue> values(6);
            values[1] = DBValue{ user.firstName() };
            values[2] = DBValue{ user.middleName() };
            values[3] = DBValue{ user.surName() };
            values[4] = DBValue{ transformToSQLiteDateString(user.birthday()) };
            values[5] = DBValue{ user.homeAddress().id() };

            auto insertResult = dal.insertQuery("users", values);
            if(holds_alternative<long long>(insertResult))
            {
                user.id(get<long long>(insertResult));
            }
        }
        else
        {
            map<string, DBValue> valueMap;
            valueMap.insert({"firstName", user.firstName()});
            valueMap.insert({"middleName", user.middleName()});
            valueMap.insert({"surName", user.surName()});
            valueMap.insert({"birthday", transformToSQLiteDateString(user.birthday())});

            string whereClause = "id=" + to_string(user.id());

            dal.updateQuery("users", valueMap, whereClause);
        }
    }

    ResultSet<User> UserRepo::getUserById(const DBId id)
    {
        DBQuery newQuery { "select * from users where id=?", 1 };
        newQuery.bindValues(id);

        return ResultSet<User>(newQuery, this);
    }

    ResultSet<User> UserRepo::getUsersBySurName(const string& surname)
    {
        DBQuery newQuery { "select * from users where surname=?", 100 };
        newQuery.bindValues(surname);

        return ResultSet<User>(newQuery, this);
    }
}