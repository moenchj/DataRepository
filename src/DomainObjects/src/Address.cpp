#include "Address.h"

using namespace std;

Address::Address()
: Address(-1ll, "", "", "", "")
{}

Address::Address(long long id, std::string street, std::string number, std::string zipcode, std::string city)
: BaseDO(id), _street(street), _number(number), _zipcode(zipcode), _city(city)
{}

const string& Address::street() const
{
    return _street;
}

const string& Address::number() const
{
    return _number;
}

const string& Address::zipcode() const
{
    return _zipcode;
}

const string& Address::city() const
{
    return _city;
}

void Address::street(const string& street)
{
    this->_street = street;
}

void Address::number(const string& number)
{
    this->_number = number;
}

void Address::zipcode(const string& zipcode)
{
    this->_zipcode = zipcode;
}

void Address::city(const string& city)
{
    this->_city = city;
}
