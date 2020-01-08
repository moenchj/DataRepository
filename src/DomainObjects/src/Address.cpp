#include "Address.h"

using namespace std;

const string& Address::street() const
{
    return _street;
}

const string& Address::number() const
{
    return _number;
}

const string& Address::zicode() const
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

void Address::zicode(const string& zicode)
{
    this->_zipcode = zicode;
}

void Address::city(const string& city)
{
    this->_city = city;
}
