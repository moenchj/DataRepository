#include "DataRepository.h"
#include "SQLiteDAL.h"

using namespace std;
namespace DO
{
    unique_ptr<DataRepository> DataRepository::_instance;
    unique_ptr<DAL::DAL> DataRepository::_dal;

    DataRepository DataRepository::instance()
    {
        if(!_instance)
        {
            _dal = make_unique<DAL::SQLiteDAL>("Database.sqlite");
            _instance = make_unique<DataRepository>(DataRepository(*_dal));
        }
        return *_instance;
    }

    DataRepository DataRepository::instance(DAL::DAL& dal)
    {
        if(!_instance)
        {
            _instance = make_unique<DataRepository>(DataRepository(dal));
        }
        return *_instance;
    }

    void DataRepository::dispose()
    {
        _dal.release();
        _instance.release();
    }

    DataRepository::DataRepository(DAL::DAL& dal)
    : _users(dal), _addresses(dal)
    {}

    UserRepo DataRepository::Users()
    {
        return _users;
    }

    AddressRepo DataRepository::Addresses()
    {
        return _addresses;
    }
};