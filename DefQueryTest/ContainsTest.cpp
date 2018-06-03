#include "gtest/gtest.h"
#include <vector>
#include "../DefQuery/from.h"
#include "../DefQuery/contains.h"

TEST(ContainsTest, IntegerCollectionTest)
{
    std::vector<int> numbers = { 1,2,3,4,5,6 };

    bool hasFive = DefQuery::from(numbers)
            .contains([](int a) { return a == 5;});

    ASSERT_TRUE(hasFive);
}

TEST(ContainsTest, NotFoundTest)
{
	std::vector<int> numbers = { 1,2,3,4,5,6 };

	bool hasFiftyFive = DefQuery::from(numbers)
		.contains([](int a) { return a == 55; });

	ASSERT_FALSE(hasFiftyFive);
}
