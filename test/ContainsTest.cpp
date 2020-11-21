#include "gtest/gtest.h"
#include <DefQuery/contains.h>
#include <DefQuery/from.h>
#include <vector>

TEST(ContainsTest, ValueSearchTest)
{
	std::vector<int> numbers = {1, 2, 3, 4, 5, 6};

	bool hasFive = DefQuery::from(numbers).contains(5);

	ASSERT_TRUE(hasFive);
}

TEST(ContainsTest, FilterTest)
{
	std::vector<int> numbers = {1, 2, 3, 4, 5, 6};

	bool hasFive = DefQuery::from(numbers).contains([](int a) { return a == 5; });

	ASSERT_TRUE(hasFive);
}

TEST(ContainsTest, NotFoundTest)
{
	std::vector<int> numbers = {1, 2, 3, 4, 5, 6};

	bool hasFiftyFive = DefQuery::from(numbers).contains([](int a) { return a == 55; });

	ASSERT_FALSE(hasFiftyFive);
}
