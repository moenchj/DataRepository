#include "UserRepo.h"

#include <sstream>
#include <string>
#include <cstdlib>
#include <regex>
#include <functional>
#include <iostream>

#include "PersistanceService.h"
#include "ResultSet.h"

using namespace std;

User UserRepo::createUser()
{
    return User();
}

Date UserRepo::createDateFromDateField(const string& dateFieldValue)
{
    regex dateRegex {"([0-9][0-9][0-9][0-9])-([0-9][0-9])-([0-9][0-9])"};
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
    list<DBRow> addressList = dal.executeQuery("select * from addresses where id=" + dbRow.values[5], 0, 1);
    if(addressList.size() > 0) 
    {
        Date birthDate = createDateFromDateField(dbRow.values[4]);
        Address homeAddress = createAddressFromDBRow(addressList.front());
        User user { atoll(dbRow.values[0].c_str()), dbRow.values[1], dbRow.values[2], dbRow.values[3], birthDate, homeAddress };
        return user;
    }
    return User {-1ll, "", "", "", Date {0, 0, 0}, Address {-1ll, "", "", "", ""} };
}

list<User> UserRepo::executeQuery(const string& query, unsigned int startAt, unsigned int numEntries)
{
    list<User> resultList;
    list<DBRow> dbList = dal.executeQuery(query, startAt, numEntries);
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
        query << "insert into addresses values (" << 
            "null" << ", \"" << 
            address.street()  << "\", \"" <<
            address.number()  << "\", \"" <<
            address.zipcode()  << "\", \"" <<
            address.city() << "\")";
            dal.executeQuery(query.str(), 0, 0);
            list<DBRow> result = dal.executeQuery("select last_insert_rowid()", 0, 1);                
            address.id(atoll(result.front().values[0].c_str()));
    }
    else
    {
        query << "update addresses set " << 
            "street=\"" << address.street()  << "\", " <<
            "number=\"" << address.number()  << "\", " <<
            "zipcode=\"" << address.zipcode()  << "\", " <<
            "city=\"" << address.city()  << "\", " <<
            " where id=" << address.id();
            dal.executeQuery(query.str(), 0, 0);
    }
        
    return address.id();
}

long long UserRepo::save(User& user)
{
    stringstream query;
    save(user._homeAddress);
    if(user.id() == -1)
    {        
        query << "insert into users values (" << 
            "null" << ", \"" << 
            user.firstName()  << "\", \"" <<
            user.middleName() << "\", \"" << 
            user.surName() << "\", \"" <<
            transformToSQLiteDateString(user.birthday()) << "\", " <<
            user.homeAddress().id() << ")";
        dal.executeQuery(query.str(), 0, 0);
        list<DBRow> result = dal.executeQuery("select last_insert_rowid()", 0, 1);                
        user.id(atoll(result.front().values[0].c_str()));
    }
    else
    {
        query << "update users set " << 
            "firstName=\"" << user.firstName()  << "\", " <<
            "middleName=\"" << user.middleName()  << "\", " <<
            "surName=\"" << user.surName()  << "\", " <<
            "birthday=\"" << transformToSQLiteDateString(user.birthday()) << "\", " <<
            " where id=" << user.id();
        dal.executeQuery(query.str(), 0, 0);
    }
    return user.id();
}

ResultSet<User> UserRepo::getUserById(const unsigned int id)
{
    stringstream query;
    query <<  "select * from users where id=" << id;

    return ResultSet<User>(query.str(), *this);
}

ResultSet<User> UserRepo::getUsersBySurName(const string& surname)
{
    stringstream query;
    query <<  "select * from users where surname=\"" << surname << "\"";
    
    return ResultSet<User>(query.str(), *this);
}
