#include "User.h"

using namespace std;

const string& User::firstName() const
{
    return _firstName;
}

const string& User::middleName() const
{
    return _middleName;
}

const string& User::surName() const
{
    return _surName;
}

const Address& User::homeAddress() const
{
    return _homeAddress;    
}

const Date& User::birthday() const
{
    return _birthday;
}

void User::firstName(const string& firstName)
{
    this->_firstName = firstName;
}

void User::middleName(const string& middleName)
{
    this->_middleName = middleName;
}

void User::surName(const string& surName)
{
    this->_surName = surName;
}

void User::homeAddress(const Address& homeAddress)
{
    this->_homeAddress = homeAddress;
}

void User::birthday(const Date& birthday)
{
    this->_birthday = birthday;
}
