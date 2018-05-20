#include "pch.h"
#include <list>
#include "../DefQuery/from.h"
#include "../DefQuery/where.h"

TEST(FilterTest, FromTest)
{
	std::list<int> lis = { 1,2,3,4,5,6 };

	auto enumerator = from(lis)
		.where([](int a) { return a < 4 || a > 5; });

	ASSERT_TRUE(enumerator.moveNext());
	ASSERT_EQ(1, enumerator.current());
	ASSERT_TRUE(enumerator.moveNext());
	ASSERT_EQ(2, enumerator.current());
	ASSERT_TRUE(enumerator.moveNext());
	ASSERT_EQ(3, enumerator.current());
	ASSERT_TRUE(enumerator.moveNext());
	ASSERT_EQ(6, enumerator.current());
	ASSERT_FALSE(enumerator.moveNext());
	ASSERT_FALSE(enumerator.moveNext());
}
