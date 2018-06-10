#include "gtest/gtest.h"
#include <list>
#include <string>
#include <sstream>
#include "../DefQuery/from.h"
#include "../DefQuery/first.h"

TEST(FirstTest, ListFirstElementTest)
{
    std::list<int> lis = { 1,2,3 };
    
    auto firstElement = DefQuery::from(lis)
        .first();
    
    ASSERT_EQ(1, firstElement);
}

TEST(FirstTest, EmptySourceEnumerationTest)
{
    std::list<int> lis;
    
    EXPECT_THROW(DefQuery::from(lis).first(), std::runtime_error);
}
