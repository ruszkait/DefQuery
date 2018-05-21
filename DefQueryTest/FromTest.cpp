#include "pch.h"
#include <list>
#include <array>
#include "../DefQuery/from.h"

TEST(FromPointerTest, FromTest)
{
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	auto enumerator = from(&arr[0], &arr[4]);

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

TEST(FromIteratorTest, FromTest)
{
	std::list<int> list = { 1,2,3 };

	auto enumerator = from(list.begin(), list.end());

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(2, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(FromContainerTest, FromTest)
{
	std::list<int> list = { 1,2,3 };

	auto enumerator = from(list);

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(2, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(FromEmptyContainerTest, FromTest)
{
	std::list<int> list;

	auto enumerator = from(list);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}