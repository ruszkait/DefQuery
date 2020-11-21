#include "gtest/gtest.h"
#include <DefQuery/any.h>
#include <DefQuery/from.h>
#include <list>

TEST(AnyTest, AnyPositiveTest)
{
	std::list<int> list = {1, 2, 3};

	auto anySmallerThan10 = DefQuery::from(list).any([](auto a) { return a < 10; });

	ASSERT_TRUE(anySmallerThan10);
}

TEST(AnyTest, AnyNegativeTest)
{
	std::list<int> list = {1, 2, 3};

	auto anyGreaterThan5 = DefQuery::from(list).any([](auto a) { return a > 5; });

	ASSERT_FALSE(anyGreaterThan5);
}
