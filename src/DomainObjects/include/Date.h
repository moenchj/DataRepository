#pragma once

#include <string>
#include "BaseDO.h"

class UserRepo;
class User;

class Date
{
    private:
        unsigned int _day;
        unsigned int _month;
        unsigned int _year;

        Date();
        Date(int day, int month, int year);
    public:       
        unsigned int day() const;
        unsigned int month() const;
        unsigned int year() const;

        void day(const unsigned int day);
        void month(const unsigned int month);
        void year(const unsigned int year);

        friend UserRepo;
        friend User;
};
