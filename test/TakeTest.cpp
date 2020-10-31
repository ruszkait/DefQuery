#include "gtest/gtest.h"
#include <list>
#include <DefQuery/from.h>
#include <DefQuery/take.h>

TEST(TakeTest, SimpleTest)
{
	std::list<int> list = { 1,2,3,4,5,6 };

	auto enumerator = DefQuery::from(list)
		.take(3);

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(2, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(TakeTest, EmptyTest)
{
	std::list<int> list;

	auto enumerator = DefQuery::from(list)
		.take(3);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(TakeTest, CopyTest)
{
	std::list<int> list = { 1,2,3,4,5,6 };

	auto enumerator = DefQuery::from(list)
		.take(3);

	auto enumeratorCopy = enumerator;

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(2, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);

	ASSERT_TRUE(++enumeratorCopy);
	ASSERT_EQ(1, *enumeratorCopy);
	ASSERT_TRUE(++enumeratorCopy);
	ASSERT_EQ(2, *enumeratorCopy);
	ASSERT_TRUE(++enumeratorCopy);
	ASSERT_EQ(3, *enumeratorCopy);
	ASSERT_FALSE(++enumeratorCopy);
	ASSERT_FALSE(++enumeratorCopy);
}

TEST(TakeTest, MoveTest)
{
	std::list<int> list = { 1,2,3,4,5,6 };

	auto enumerator = DefQuery::from(list)
		.take(3);

	auto enumerator2 = std::move(enumerator);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(1, *enumerator2);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(2, *enumerator2);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(3, *enumerator2);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);
}
