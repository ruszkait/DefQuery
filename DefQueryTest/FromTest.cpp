#include "gtest/gtest.h"
#include <list>
#include <vector>
#include "../DefQuery/from.h"
#include "../DefQuery/count.h"

TEST(FromTest, FromContainerTest)
{
	std::list<int> list = { 1,2,3 };

	// Warning: the from_enumerator does not own or make a copy of the collection, so make sure
	// not to pass a temporal variable as the parameter, because it gets destroyed immediately
	// after the from enumerator is created.
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

TEST(FromTest, CopyTest)
{
	std::list<int> list = { 1,2,3 };

	auto enumerator = DefQuery::from(list);
	auto enumeratorCopy = enumerator;

	ASSERT_EQ(3, enumerator.count());
	ASSERT_EQ(3, enumeratorCopy.count());
}

TEST(FromTest, MoveTest)
{
	std::list<int> list = { 1,2,3 };

	auto enumerator = DefQuery::from(list);
	auto enumerator2 = std::move(enumerator);

	ASSERT_EQ(0, enumerator.count());
	ASSERT_EQ(3, enumerator2.count());
}
