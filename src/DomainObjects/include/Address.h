#pragma once

#include <string>
#include "BaseDO.h"

class UserRepo;
class User;

class Address : public BaseDO
{
    private:
        std::string _street;
        std::string _number;
        std::string _zipcode;
        std::string _city;

        Address();
        Address(long long id, std::string street, std::string number, std::string zipcode, std::string city);

    public:
        const std::string& street() const;
        const std::string& number() const;
        const std::string& zipcode() const;
        const std::string& city() const;

        void street(const std::string& street);
        void number(const std::string& number);
        void zipcode(const std::string& zipcode);
        void city(const std::string& city);

        friend UserRepo;
        friend User;
};
