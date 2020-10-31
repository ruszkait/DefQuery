#include "gtest/gtest.h"
#include <list>
#include <vector>
#include <DefQuery/range.h>

TEST(RangeTest, EmptyRangeTest)
{
	auto enumerator = DefQuery::range(1, 1);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(RangeTest, IncreasingNumberTest)
{
	auto enumerator = DefQuery::range(1, 5);

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

TEST(RangeTest, SteppingNumberTest)
{
	auto enumerator = DefQuery::range(1, 12)
		.step(3);

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(4, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(7, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(10, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(RangeTest, PointerTest)
{
	std::vector<int> vector = { 1,2,3,4,5,6 };

	auto enumerator = DefQuery::range(&vector[0], &vector[4]);

	// The right side asterix in **enumerator dereferences the enumerator, the left side asterix dereferences the delivered pointer
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, **enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(2, **enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, **enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(4, **enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(RangeTest, IteratorTest)
{
	std::list<int> list = { 1,2,3 };

	auto enumerator = DefQuery::range(list.begin(), list.end());

	// The right side asterix in **enumerator dereferences the enumerator, the left side asterix dereferences the delivered iterator
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, **enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(2, **enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, **enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(RangeTest, CopyTest)
{
	auto enumerator = DefQuery::range(1, 4);

	auto enumerator2 = enumerator;

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(2, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
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

TEST(RangeTest, MoveTest)
{
	auto enumerator = DefQuery::range(1, 4);

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
