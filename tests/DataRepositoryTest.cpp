#include <string>
#include <iostream>
#include <list>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "DataRepository.h"

using namespace std;
using namespace testing;

class DataRepositoryTest : public ::testing::Test {

  protected:
    virtual void SetUp() {
        DataRepository::dispose();
    }

    virtual void TearDown() {
    }
};

class DALMock : public DAL
{
    public:
        MOCK_METHOD((list<DBRow>), selectQuery, (const string&, unsigned int, unsigned int));
        MOCK_METHOD((variant<monostate, long long>), insertQuery, (const string&, (const vector<DBInsertValue>&)));
        MOCK_METHOD(void, updateQuery, (const string&, (const map<string, DBValue>&), const string&));
};

TEST_F(DataRepositoryTest, BasicFunction)
{
    DALMock dalMock;
    DBRow userTableRow(6);
    userTableRow.values[0] = "1";
    userTableRow.values[1] = "Joanne";
    userTableRow.values[2] = "Kathleen";
    userTableRow.values[3] = "Rowling";
    userTableRow.values[4] = "1965-07-31";
    userTableRow.values[5] = "1";
    DBRow addressTableRow(5);
    addressTableRow.values[0] = "1";
    addressTableRow.values[1] = "Diagon Alley";
    addressTableRow.values[2] = "10";
    addressTableRow.values[3] = "WC2N 5DU";
    addressTableRow.values[4] = "London";
    EXPECT_CALL(dalMock, selectQuery("select * from users where id=1", 0, 100)).Times(1).WillOnce(Return(list<DBRow>{ userTableRow }));
    EXPECT_CALL(dalMock, selectQuery("select * from addresses where id=1", 0, 1)).Times(1).WillOnce(Return(list<DBRow>{ addressTableRow }));
    DataRepository dataRepo = DataRepository::instance(dalMock);
    ResultSet<User> result = dataRepo.Users().getUserById(1);
    User user = result.front();
    ASSERT_EQ(user.firstName(), "Joanne");
    ASSERT_EQ(user.middleName(), "Kathleen");
    ASSERT_EQ(user.surName(), "Rowling");
    ASSERT_EQ(user.birthday().day(), 31);
    ASSERT_EQ(user.birthday().month(), 7);
    ASSERT_EQ(user.birthday().year(), 1965);
    ASSERT_EQ(user.homeAddress().street(), "Diagon Alley");
    ASSERT_EQ(user.homeAddress().number(), "10");
    ASSERT_EQ(user.homeAddress().zipcode(), "WC2N 5DU");
    ASSERT_EQ(user.homeAddress().city(), "London");
}
