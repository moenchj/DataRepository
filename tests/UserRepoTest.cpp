#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "DataRepository.h"

using namespace std;
using namespace testing;
using namespace DO;

class UserRepoTest : public ::testing::Test {

  protected:
    virtual void SetUp() {
        DataRepository::dispose();
    }

    virtual void TearDown() {
    }
};

TEST_F(UserRepoTest, SaveAndGet)
{
    UserRepo users = DataRepository::instance().Users();
    User newUser = users.createUser();
    newUser.firstName("Frank");
    newUser.middleName("N");
    newUser.surName("Stein");
    Address homeAddress = newUser.homeAddress();
    homeAddress.street("Wallstreet");
    homeAddress.number("1");
    homeAddress.zipcode("57 ACF 63");
    homeAddress.city("New York");
    newUser.homeAddress(homeAddress);
    Date birthDay = newUser.birthday();
    birthDay.set(2, 3, 1998);
    newUser.birthday(birthDay);

    try
    {
        users.save(newUser);
        ResultSet<User> result = users.getUsersBySurName("Stein");
        ASSERT_EQ(result.front().surName(), "Stein");
        ASSERT_EQ(result.front().birthday().year(), 1998);
        ASSERT_EQ(result.front().birthday().month(), 3);
        ASSERT_EQ(result.front().birthday().day(), 2);
    }
    catch(const exception& e)
    {
        cout << "Exception during test! " << e.what() << endl;
    }
    catch(...)
    {
        cout << "Exception during test!" << endl;
    }
}

TEST_F(UserRepoTest, UpdateFunction)
{
    UserRepo users = DataRepository::instance().Users();
    ResultSet<User> result = users.getUsersBySurName("Stein");
    for(auto user: result)
    {
        user.surName("Granit");
        users.save(user);
    }

    try
    {
        result = users.getUsersBySurName("Stein");
        ASSERT_EQ(result.size(), 0);
        result = users.getUsersBySurName("Granit");
        ASSERT_EQ(result.front().surName(), "Granit");
        ASSERT_EQ(result.front().birthday().year(), 1998);
        ASSERT_EQ(result.front().birthday().month(), 3);
        ASSERT_EQ(result.front().birthday().day(), 2);
    }
    catch(const exception& e)
    {
        cout << "Exception during test! " << e.what() << endl;
    }
    catch(...)
    {
        cout << "Exception during test!" << endl;
    }
}
