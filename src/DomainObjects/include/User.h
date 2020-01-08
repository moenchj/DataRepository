#pragma once

#include <string>
#include "Address.h"
#include "Date.h"
#include "BaseDO.h"

class User : public BaseDO
{
    private:
        std::string _firstName;
        std::string _middleName;
        std::string _surName;
        Address _homeAddress;
        Date _birthday;

    public:
        const std::string& firstName() const;
        const std::string& middleName() const;
        const std::string& surName() const;
        const Address& homeAddress() const;
        const Date& birthday() const;

        void firstName(const std::string& firstName);
        void middleName(const std::string& middleName);
        void surName(const std::string& surName);
        void homeAddress(const Address& homeAddress);
        void birthday(const Date& birthday);
};
