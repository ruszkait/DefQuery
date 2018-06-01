#include "pch.h"
#include <list>
#include <array>
#include "../DefQuery/from.h"

TEST(FromTest, FromPointerTest)
{
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	auto enumerator = DefQuery::from(&arr[0], &arr[4]);

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(2, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(4, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(FromTest, FromIteratorTest)
{
    std::list<int> list = { 1,2,3 };

	auto enumerator = DefQuery::from(list.begin(), list.end());

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(2, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(FromTest, FromContainerTest)
{
	std::list<int> list = { 1,2,3 };

	auto enumerator = DefQuery::from(list);

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(2, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(FromTest, FromEmptyContainerTest)
{
	std::list<int> list;

	auto enumerator = DefQuery::from(list);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}
