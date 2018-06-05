#include "gtest/gtest.h"
#include <list>
#include <array>
#include "../DefQuery/from.h"
#include "../DefQuery/list.h"

TEST(ListTest, SubCollectionTest)
{
    std::array<int, 10> arr = { 1,2,3,4,5,6 };
    
    auto subArrList = DefQuery::from(&arr[0], &arr[4])
        .list();
    
    ASSERT_EQ(std::list<int>({1,2,3,4}), subArrList);
}

TEST(ListTest, EmptyCollectionTest)
{
    std::array<int, 10> arr = { 1,2,3,4,5,6 };
    
    auto subArrList = DefQuery::from(&arr[0], &arr[0])
        .list();
    
    ASSERT_TRUE(subArrList.empty());
}
