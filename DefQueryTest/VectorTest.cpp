#include "gtest/gtest.h"
#include <list>
#include <array>
#include "../DefQuery/from.h"
#include "../DefQuery/vector.h"

TEST(VectorTest, SubCollectionTest)
{
    std::array<int, 10> arr = { 1,2,3,4,5,6 };
    
    auto subArrVector = DefQuery::from(&arr[0], &arr[4])
        .vector();
    
    ASSERT_EQ(std::vector<int>({1,2,3,4}), subArrVector);
}

TEST(VectorTest, EmptyCollectionTest)
{
    std::array<int, 10> arr = { 1,2,3,4,5,6 };
    
    auto subArrVector = DefQuery::from(&arr[0], &arr[0])
        .vector();
    
    ASSERT_TRUE(subArrVector.empty());
}
