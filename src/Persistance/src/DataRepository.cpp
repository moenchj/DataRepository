#include "DataRepository.h"
#include "DemoDAL.h"

using namespace std;

unique_ptr<DataRepository> DataRepository::_instance;
unique_ptr<DAL> DataRepository::_dal;

DataRepository DataRepository::instance()
{
    if(!_instance)
    {
        _dal = make_unique<DemoDAL>();
        _instance = make_unique<DataRepository>(DataRepository(*_dal));
    }
    return *_instance;
}

DataRepository::DataRepository(DAL& dal) :
    _users(dal)
{}

UserRepo DataRepository::Users()
{
    return _users;
}