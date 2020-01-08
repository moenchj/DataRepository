#include "UserRepo.h"

#include <sstream>
#include <string>

#include "PersistanceService.h"
#include "ResultSet.h"

using namespace std;

ResultSet<User> UserRepo::getUserById(const unsigned int id)
{
    stringstream query;
    query <<  "select * from user where id=" << id;

    return ResultSet<User>(query.str(), *this);
}

ResultSet<User> UserRepo::getUsersBySurName(const string& surname)
{
    stringstream query;
    query <<  "select * from user where surname=" << surname;
    
    return ResultSet<User>(query.str(), *this);
}
