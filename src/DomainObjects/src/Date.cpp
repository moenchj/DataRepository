#include "Date.h"

unsigned int Date::day() const
{
    return _day;
}

unsigned int Date::month() const
{
    return _month;
}

unsigned int Date::year() const
{
    return _year;
}

void Date::day(const unsigned int day)
{
    this->_day = day;
}

void Date::month(const unsigned int month)
{
    this->_month = month;
}

void Date::year(const unsigned int year)
{
    this->_year = year;
}
