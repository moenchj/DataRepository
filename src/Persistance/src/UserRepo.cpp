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

using namespace std;

User UserRepo::createUser()
{
    return User();
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

Address UserRepo::createAddressFromDBRow(const DBRow& dbRow)
{
    Address address { atoll(dbRow.values[0].c_str()), dbRow.values[1], dbRow.values[2], dbRow.values[3], dbRow.values[4] };
    return address;
}

User UserRepo::createUserFromDBRow(const DBRow& dbRow)
{
    list<DBRow> addressList = dal.selectQuery("select * from addresses where id=" + dbRow.values[5], 0, 1);
    Address homeAddress;
    if(addressList.size() > 0) 
    {
        homeAddress = createAddressFromDBRow(addressList.front());
    }
    Date birthDate = createDateFromDateField(dbRow.values[4]);
    User user { atoll(dbRow.values[0].c_str()), dbRow.values[1], dbRow.values[2], dbRow.values[3], birthDate, homeAddress };
    return user;
}

list<User> UserRepo::select(const string& query, unsigned int startAt, unsigned int numEntries)
{
    list<User> resultList;
    list<DBRow> dbList = dal.selectQuery(query, startAt, numEntries);
    transform (dbList.begin(), dbList.end(), back_inserter(resultList), bind(mem_fn(&UserRepo::createUserFromDBRow), this, placeholders::_1));
    return resultList;
}

string UserRepo::transformToSQLiteDateString(const Date& date)
{
    stringstream dateString;
    dateString << date.year() << "-" << date.month() << "-" << date.day();
    return dateString.str();
}

long long UserRepo::save(Address& address)
{
    stringstream query;
    if(address.id() == -1)
    {
        vector<DBInsertValue> values(5);
        values[1] = DBValue{ address.street() };
        values[2] = DBValue{ address.number() };
        values[3] = DBValue{ address.zipcode() };
        values[4] = DBValue{ address.city() };

        auto insertResult = dal.insertQuery("addresses", values);
        if(holds_alternative<long long>(insertResult))
        {
            address.id(get<long long>(insertResult));
        }
    }
    else
    {
        map<string, DBValue> valueMap;
        valueMap.insert({"street", address.street()});
        valueMap.insert({"number", address.number()});
        valueMap.insert({"zipcode", address.zipcode()});
        valueMap.insert({"city", address.city()});

        string whereClause = "id=" + to_string(address.id());

        dal.updateQuery("addresses", valueMap, whereClause);
    }
        
    return address.id();
}

long long UserRepo::save(User& user)
{
    stringstream query;
    save(user._homeAddress);
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
    return user.id();
}

ResultSet<User> UserRepo::getUserById(const unsigned int id)
{
    stringstream query;
    query <<  "select * from users where id=" << id;

    return ResultSet<User>(query.str(), this);
}

ResultSet<User> UserRepo::getUsersBySurName(const string& surname)
{
    stringstream query;
    query <<  "select * from users where surname=\"" << surname << "\"";
    
    return ResultSet<User>(query.str(), this);
}
