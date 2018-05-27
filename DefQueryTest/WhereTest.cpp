#include "pch.h"
#include <list>
#include "../DefQuery/from.h"
#include "../DefQuery/where.h"

TEST(WhereTest, FilterTest)
{
	std::list<int> lis = { 1,2,3,4,5,6 };

	auto enumerator = DefQuery::from(lis)
		.where([](const int a) { return a < 4 || a > 5; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(2, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(6, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}
