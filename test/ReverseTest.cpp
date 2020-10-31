#include "gtest/gtest.h"
#include <list>
#include <DefQuery/from.h>
#include <DefQuery/reverse.h>

TEST(ReverseTest, ReverseTest)
{
	std::list<int> list = { 1,3,7 };

	auto enumerator = DefQuery::from(list)
		.reverse();

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(7, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(ReverseTest, EmptyTest)
{
	std::list<int> list;

	auto enumerator = DefQuery::from(list)
		.reverse();

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(ReverseTest, CopyTest)
{
	std::list<int> list = { 1,3,7 };

	auto enumerator = DefQuery::from(list)
		.reverse();

	auto enumerator2 = enumerator;

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(7, *enumerator2);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(3, *enumerator2);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(1, *enumerator2);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(7, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(ReverseTest, CopyStartedTest)
{
	std::list<int> list = { 1,3,7 };

	auto enumerator = DefQuery::from(list)
		.reverse();

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(7, *enumerator);

	auto enumerator2 = enumerator;

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(3, *enumerator2);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(1, *enumerator2);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(ReverseTest, MoveTest)
{
	std::list<int> list = { 1,3,7 };

	auto enumerator = DefQuery::from(list)
		.reverse();

	auto enumerator2 = std::move(enumerator);

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(7, *enumerator2);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(3, *enumerator2);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(1, *enumerator2);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}


TEST(ReverseTest, MoveStartedTest)
{
	std::list<int> list = { 1,3,7 };

	auto enumerator = DefQuery::from(list)
		.reverse();

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(7, *enumerator);

	auto enumerator2 = std::move(enumerator);

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(3, *enumerator2);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(1, *enumerator2);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}
