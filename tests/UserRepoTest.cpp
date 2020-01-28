#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "DataRepository.h"

using namespace std;
using namespace testing;

class UserRepoTest : public ::testing::Test {

  protected:
    virtual void SetUp() {
        DataRepository::dispose();
    }

    virtual void TearDown() {
    }
};

TEST_F(UserRepoTest, BasicFunction)
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
    birthDay.day(2);
    birthDay.month(3);
    birthDay.year(1998);
    newUser.birthday(birthDay);

    try
    {
        users.save(newUser);
        ResultSet<User> result = users.getUsersBySurName("Stein");
        ASSERT_EQ(result.front().surName(), "Stein");
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
