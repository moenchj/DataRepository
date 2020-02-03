#include "User.h"
#include "Date.h"

using namespace std;

namespace DO
{
    User::User(PersistanceService<User>* pPersistanceService, long long id, const std::string& firstName, const std::string& middleName, const std::string& surName, const Date& birthday, const Address& homeAddress)
    : BaseDO(pPersistanceService, id), _firstName(firstName), _middleName(middleName), _surName(surName), _birthday(birthday), _homeAddress(homeAddress)
    {}

    const string& User::firstName() const
    {
        return _firstName;
    }

    const string& User::middleName() const
    {
        return _middleName;
    }

    const string& User::surName() const
    {
        return _surName;
    }

    const Address& User::homeAddress() const
    {
        return _homeAddress;    
    }

    const Date& User::birthday() const
    {
        return _birthday;
    }

    void User::firstName(const string& firstName)
    {
        this->_firstName = firstName;
        setDirty();
    }

    void User::middleName(const string& middleName)
    {
        this->_middleName = middleName;
        setDirty();
    }

    void User::surName(const string& surName)
    {
        this->_surName = surName;
        setDirty();
    }

    void User::homeAddress(const Address& homeAddress)
    {
        this->_homeAddress = homeAddress;
        setDirty();
    }

    void User::birthday(const Date& birthday)
    {
        this->_birthday = birthday;
        setDirty();
    }
};