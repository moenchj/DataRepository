#pragma once

#include <string>
#include "BaseDO.h"

class Address
{
    private:
        std::string _street;
        std::string _number;
        std::string _zipcode;
        std::string _city;
    public:
        const std::string& street() const;
        const std::string& number() const;
        const std::string& zicode() const;
        const std::string& city() const;

        void street(const std::string& street);
        void number(const std::string& number);
        void zicode(const std::string& zicode);
        void city(const std::string& city);
};
