#include "gtest/gtest.h"
#include <DefQuery/first.h>
#include <DefQuery/from.h>
#include <list>
#include <sstream>
#include <string>

TEST(FirstTest, ListFirstElementTest)
{
	std::list<int> list = {1, 2, 3};

	auto firstElement = DefQuery::from(list).first();

	ASSERT_EQ(1, firstElement);
}

TEST(FirstTest, FirstOrDefaultFirstElementTest)
{
	std::list<int> list = {1, 2, 3};

	auto firstElement = DefQuery::from(list).first_or_default();

	ASSERT_EQ(1, firstElement);
}

TEST(FirstTest, EmptySourceEnumerationTest)
{
	std::list<int> list;

	EXPECT_THROW(DefQuery::from(list).first(), std::runtime_error);
}

TEST(FirstTest, FirstOrDefaultEmptySourceEnumerationTest)
{
	std::list<int> list;

	auto defaultElement = DefQuery::from(list).first_or_default();

	ASSERT_EQ(0, defaultElement);
}
