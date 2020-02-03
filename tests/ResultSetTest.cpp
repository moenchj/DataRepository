#include <string>
#include <iostream>
#include <list>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ResultSet.h"
#include "DBQuery.h"

using namespace std;
using namespace testing;
using namespace DO;
using namespace DAL;

class DummyBO
{
    private:
        string text;

    public:
        DummyBO(const string& text)
        : text(text)
        {}

        bool operator==(const string& other) const
        {
            return text == other;
        }
};

class PersistanceServiceMock : public PersistanceService<DummyBO>
{
    public:
        MOCK_METHOD(void, save, (DummyBO&));
        MOCK_METHOD((list<DummyBO>), select, (const DBQuery&, const size_t startAt));
};

TEST(ResultSetTest, BasicFunction)
{
    PersistanceServiceMock psMock;
    EXPECT_CALL(psMock, select(_,_)).Times(1).WillOnce(Return(list<DummyBO>{}));
    ResultSet<DummyBO> resultSet { DBQuery("BasicFunction query", 1), &psMock };
}

TEST(ResultSetTest, OneResult)
{
    PersistanceServiceMock psMock;
    EXPECT_CALL(psMock, select(_,_)).Times(1).WillOnce(Return(list<DummyBO>{DummyBO{"The first string"}}));
    ResultSet<DummyBO> resultSet { DBQuery("OneResult query", 2), &psMock };
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
    EXPECT_CALL(psMock, select(_,_)).Times(1).WillOnce(Return(list<DummyBO>{DummyBO{"The first string"}, DummyBO{"The second string"}}));
    ResultSet<DummyBO> resultSet { DBQuery("TwoResults query", 3), &psMock };
    int numResults = 0;
    for(auto value: resultSet)
    {
        numResults++;
    }
    ASSERT_EQ(numResults, 2);
}

TEST(ResultSetTest, TwoPages)
{
    const size_t pageSize = 2;
    PersistanceServiceMock psMock;
    EXPECT_CALL(psMock, select(_,0)).Times(1).
        WillOnce(Return(list<DummyBO>{DummyBO{"The first string"}, DummyBO{"The second string"}}));
    EXPECT_CALL(psMock, select(_,2)).Times(1).
        WillOnce(Return(list<DummyBO>{DummyBO{"The third string"}}));
    ResultSet<DummyBO> resultSet { DBQuery("TwoResults query", 2), &psMock };
    int numResults = 0;
    for(auto value: resultSet)
    {
        numResults++;
    }
    ASSERT_EQ(numResults, 3);
}

TEST(ResultSetTest, TwoFullPages)
{
    const size_t pageSize = 2;
    PersistanceServiceMock psMock;
    EXPECT_CALL(psMock, select(_,0)).Times(1).
        WillOnce(Return(list<DummyBO>{DummyBO{"The first string"}, DummyBO{"The second string"}}));
    EXPECT_CALL(psMock, select(_,2)).Times(1).
        WillOnce(Return(list<DummyBO>{DummyBO{"The third string"}, DummyBO{"The fourth string"}}));
    EXPECT_CALL(psMock, select(_,4)).Times(1).
        WillOnce(Return(list<DummyBO>{}));
    ResultSet<DummyBO> resultSet { DBQuery("TwoFullPages query", 2), &psMock };
    int numResults = 0;
    for(ResultSet<DummyBO>::iterator iter = resultSet.begin();
        iter != resultSet.end();
        iter++)
    {
        numResults++;
    }
    ASSERT_EQ(numResults, 4);
}
