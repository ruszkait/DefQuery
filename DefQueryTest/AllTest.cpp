#include "gtest/gtest.h"
#include <list>
#include "../DefQuery/from.h"
#include "../DefQuery/all.h"

TEST(AllTest, AllPositiveTest)
{
	std::list<int> list = { 1,2,3 };

	auto allSmallerThan10 = DefQuery::from(list)
		.all([](auto a) { return a < 10; });

	ASSERT_TRUE(allSmallerThan10);
}

TEST(AllTest, AllNegativeTest)
{
	std::list<int> list = { 1,2,3 };

	auto allGreaterThan2 = DefQuery::from(list)
		.all([](auto a) { return a > 2; });

	ASSERT_FALSE(allGreaterThan2);
}

