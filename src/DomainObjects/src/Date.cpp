#include "Date.h"

namespace DO
{
    Date::Date(int day, int month, int year)
    {
        set(day, month, year);
    }

    int Date::day() const
    {
        return _day;
    }

    int Date::month() const
    {
        return _month;
    }

    int Date::year() const
    {
        return _year;
    }

    void Date::set(const int day, const int month, const int year)
    {
        if(day <= daysInMonth(month, year) && month >= 1 && month <= 12)
        {
            _day = day;
            _month = month;
            _year = year;
        }
    }

    unsigned int Date::daysInMonth(const int month, const int year) const
    {
        if(month == 1 ||
           month == 5 ||
           month == 7 ||
           month == 8 ||
           month == 10 ||
           month == 12)
        {
            return 31;
        }
        else if(month == 2)
        {
            if(isLeapYear(year))
            {
                return 29;
            }
            else
            {
                return 28;
            }            
        }
        return 30;
    }

    bool Date::isLeapYear(int year) const
    {
        return ((year % 4 == 0 && year % 100 != 0) || 
                year % 400 == 0);
    }
};