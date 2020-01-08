#pragma once

#include <string>
#include "BaseDO.h"

class Date
{
    private:
        unsigned int _day;
        unsigned int _month;
        unsigned int _year;
    public:
        unsigned int day() const;
        unsigned int month() const;
        unsigned int year() const;

        void day(const unsigned int day);
        void month(const unsigned int month);
        void year(const unsigned int year);
};
