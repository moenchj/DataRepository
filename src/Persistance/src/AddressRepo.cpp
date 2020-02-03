#include "AddressRepo.h"

#include <sstream>
#include <string>
#include <cstdlib>
#include <regex>
#include <functional>
#include <iostream>
#include <cstdlib>

#include "PersistanceService.h"
#include "ResultSet.h"
#include "DBQuery.h"

using namespace std;
using namespace DAL;

namespace DO
{
    Address AddressRepo::createAddress()
    {
        return Address(this, -1ll, "", "", "", "");
    }

    Address AddressRepo::createAddressFromDBRow(const DBRow& dbRow)
    {
        Address address { this,
                          get<long long>(dbRow.values[0]), 
                          get<string>(dbRow.values[1]), 
                          get<string>(dbRow.values[2]), 
                          get<string>(dbRow.values[3]), 
                          get<string>(dbRow.values[4]) };
        return address;
    }

    list<Address> AddressRepo::select(const DBQuery& query, const size_t startAt)
    {
        list<Address> resultList;
        list<DBRow> dbList = dal.selectQuery(query, startAt);
        transform (dbList.begin(), dbList.end(), back_inserter(resultList), bind(mem_fn(&AddressRepo::createAddressFromDBRow), this, placeholders::_1));
        return resultList;
    }

    void AddressRepo::save(Address& address)
    {
        stringstream query;
        if(address.id() == -1)
        {
            vector<DBInsertValue> values(5);
            values[1] = DBValue{ address.street() };
            values[2] = DBValue{ address.number() };
            values[3] = DBValue{ address.zipcode() };
            values[4] = DBValue{ address.city() };

            auto insertResult = dal.insertQuery("addresses", values);
            if(holds_alternative<long long>(insertResult))
            {
                address.id(get<long long>(insertResult));
            }
        }
        else
        {
            map<string, DBValue> valueMap;
            valueMap.insert({"street", address.street()});
            valueMap.insert({"number", address.number()});
            valueMap.insert({"zipcode", address.zipcode()});
            valueMap.insert({"city", address.city()});

            string whereClause = "id=" + to_string(address.id());

            dal.updateQuery("addresses", valueMap, whereClause);
        }
    }

    ResultSet<Address> AddressRepo::getAddressById(const DBId id)
    {
        DBQuery newQuery { "select * from addresses where id=?", 1 };
        newQuery.bindValues(id);

        return ResultSet<Address>(newQuery, this);
    }

    ResultSet<Address> AddressRepo::getAddressByStreet(const string& street)
    {
        DBQuery newQuery { "select * from addresses where street=?", 100 };
        newQuery.bindValues(street);

        return ResultSet<Address>(newQuery, this);
    }
}