#pragma once

#include <string>
#include "Address.h"
#include "Date.h"
#include "BaseDO.h"

namespace DO
{
    class UserRepo;

    class User : public BaseDO<User>
    {
        private:
            std::string _firstName;
            std::string _middleName;
            std::string _surName;
            Address _homeAddress;
            Date _birthday;

            User(PersistanceService<User>* pPersistanceService, long long id, const std::string& firstName, const std::string& middleName, const std::string& surName, const Date& birthday, const Address& homeAddress);

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

            friend UserRepo;
    };
};