#pragma once

#include <string>
#include "BaseDO.h"

namespace DO
{
    class UserRepo;
    class User;

    class Date
    {
        private:
            int _day;
            int _month;
            int _year;

            Date(int day, int month, int year);
            bool isLeapYear(int year) const;
            unsigned int daysInMonth(const int month, const int year) const;
        public:       
            int day() const;
            int month() const;
            int year() const;

            void set(const int day, const int month, const int year);

            friend UserRepo;
            friend User;
    };
};