#include <string>
#include <iostream>
#include <list>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ResultSet.h"

using namespace std;
using namespace testing;

class PersistanceServiceMock : public PersistanceService<string>
{
    public:
        MOCK_METHOD(long long, save, (string&));
        MOCK_METHOD((list<string>), executeQuery, (const string& query, unsigned int, unsigned int));
};

TEST(ResultSetTest, BasicFunction)
{
    PersistanceServiceMock psMock;
    EXPECT_CALL(psMock, executeQuery("BasicFunction query", 0, 100)).Times(1).WillOnce(Return(list<string>{}));
    ResultSet<string> resultSet { "BasicFunction query", psMock };
}

TEST(ResultSetTest, OneResult)
{
    PersistanceServiceMock psMock;
    EXPECT_CALL(psMock, executeQuery("OneResult query", 0, 100)).Times(1).WillOnce(Return(list<string>{"The first string"}));
    ResultSet<string> resultSet { "OneResult query", psMock };
    int numResults = 0;
    for(auto value: resultSet)
    {
        numResults++;
        ASSERT_EQ(value, "The first string");
    }
    ASSERT_EQ(numResults, 1);
}

TEST(ResultSetTest, TwoResults)
{
    PersistanceServiceMock psMock;
    EXPECT_CALL(psMock, executeQuery("TwoResults query", 0, 100)).Times(1).WillOnce(Return(list<string>{"The first string", "The second string"}));
    ResultSet<string> resultSet { "TwoResults query", psMock };
    int numResults = 0;
    for(string value: resultSet)
    {
        numResults++;
    }
    ASSERT_EQ(numResults, 2);
}

TEST(ResultSetTest, TwoPages)
{
    const unsigned int pageSize = 2;
    PersistanceServiceMock psMock;
    EXPECT_CALL(psMock, executeQuery("TwoResults query", 0, pageSize)).Times(1).
        WillOnce(Return(list<string>{"The first string", "The second string"}));
    EXPECT_CALL(psMock, executeQuery("TwoResults query", 2, pageSize)).Times(1).
        WillOnce(Return(list<string>{"The third string"}));
    ResultSet<string, pageSize> resultSet { "TwoResults query", psMock };
    int numResults = 0;
    for(string value: resultSet)
    {
        numResults++;
    }
    ASSERT_EQ(numResults, 3);
}

TEST(ResultSetTest, TwoFullPages)
{
    const unsigned int pageSize = 2;
    PersistanceServiceMock psMock;
    EXPECT_CALL(psMock, executeQuery("TwoFullPages query", 0, pageSize)).Times(1).
        WillOnce(Return(list<string>{"The first string", "The second string"}));
    EXPECT_CALL(psMock, executeQuery("TwoFullPages query", 2, pageSize)).Times(1).
        WillOnce(Return(list<string>{"The third string", "The fourth string"}));
    EXPECT_CALL(psMock, executeQuery("TwoFullPages query", 4, pageSize)).Times(1).
        WillOnce(Return(list<string>{}));
    ResultSet<string, pageSize> resultSet { "TwoFullPages query", psMock };
    int numResults = 0;
    for(ResultSet<string, pageSize>::iterator iter = resultSet.begin();
        iter != resultSet.end();
        iter++)
    {
        numResults++;
    }
    ASSERT_EQ(numResults, 4);
}
