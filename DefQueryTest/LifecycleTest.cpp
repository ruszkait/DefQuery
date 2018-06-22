#include "gtest/gtest.h"
#include <list>
#include <regex>
#include <string>
#include <ctime>
#include "../DefQuery/from.h"
#include "../DefQuery/range.h"
#include "../DefQuery/stlrange.h"
#include "../DefQuery/where.h"
#include "../DefQuery/select.h"
#include "../DefQuery/orderby.h"
#include "../DefQuery/decay.h"

TEST(LifecycleTest, DuplicateQueryTest)
{
	std::list<int> list = { 1,2,3,4,5,6 };

	// With the fluent call API the previous enumerator stage will be moved into the next enumerator stage
	auto originalEnumerator = DefQuery::from(list)
		.where([](const int a) { return a > 3 && a < 6; })
		.select([](const int a) { return a * 10; });

	// A copy of the enumerator is independent of the original one, they do not share state after
	// the copy is created
	auto copiedEnumerator = originalEnumerator;

	ASSERT_TRUE(++originalEnumerator);
	ASSERT_EQ(40, *originalEnumerator);
	ASSERT_TRUE(++originalEnumerator);
	ASSERT_EQ(50, *originalEnumerator);
	ASSERT_FALSE(++originalEnumerator);
	ASSERT_FALSE(++originalEnumerator);

	// yet another copy, but this time from the first copy (copiedEnumerator), because the original one has already ran down
	auto twiceCopiedEnumerator = copiedEnumerator;

	ASSERT_TRUE(++copiedEnumerator);
	ASSERT_EQ(40, *copiedEnumerator);
	ASSERT_TRUE(++copiedEnumerator);
	ASSERT_EQ(50, *copiedEnumerator);
	ASSERT_FALSE(++copiedEnumerator);
	ASSERT_FALSE(++copiedEnumerator);

	ASSERT_TRUE(++twiceCopiedEnumerator);
	ASSERT_EQ(40, *twiceCopiedEnumerator);
	ASSERT_TRUE(++twiceCopiedEnumerator);
	ASSERT_EQ(50, *twiceCopiedEnumerator);
	ASSERT_FALSE(++twiceCopiedEnumerator);
	ASSERT_FALSE(++twiceCopiedEnumerator);
}

TEST(LifecycleTest, ForkQueryTest)
{
	std::list<int> list = { 1,6,3,8,9,2,7,5,4 };

	auto originalEnumerator = DefQuery::from(list)
		.where([](const int a) { return a > 3 && a < 9; })
		.orderby(DefQuery::sorting_order::ascending);

	// As the source enumerator (originalEnumerator) is not an rvalue, but an lvalue,
	// the enumerator stages from it are copied to the extendedEnumerator.
	// Appending further stages (extendedEnumerator) are again moves the previous stages.
	auto extendedEnumerator = originalEnumerator
		.where([](const int a) { return a % 2 == 0; })
		.select([](const int a) { return a * 10; });

	ASSERT_TRUE(++originalEnumerator);
	ASSERT_EQ(4, *originalEnumerator);
	ASSERT_TRUE(++originalEnumerator);
	ASSERT_EQ(5, *originalEnumerator);
	ASSERT_TRUE(++originalEnumerator);
	ASSERT_EQ(6, *originalEnumerator);
	ASSERT_TRUE(++originalEnumerator);
	ASSERT_EQ(7, *originalEnumerator);
	ASSERT_TRUE(++originalEnumerator);
	ASSERT_EQ(8, *originalEnumerator);
	ASSERT_FALSE(++originalEnumerator);
	ASSERT_FALSE(++originalEnumerator);

	ASSERT_TRUE(++extendedEnumerator);
	ASSERT_EQ(40, *extendedEnumerator);
	ASSERT_TRUE(++extendedEnumerator);
	ASSERT_EQ(60, *extendedEnumerator);
	ASSERT_TRUE(++extendedEnumerator);
	ASSERT_EQ(80, *extendedEnumerator);
	ASSERT_FALSE(++extendedEnumerator);
	ASSERT_FALSE(++extendedEnumerator);
}

TEST(LifecycleTest, ForkQueryWithDecayTest)
{
	std::list<int> list = { 1,6,3,8,9,2,7,5,4 };

	// Decay an rvalue -> the rvalue enumerator chain will be moved to the heap
	DefQuery::decayed_enumerator<int> originalEnumerator = DefQuery::from(list)
		.where([](const int a) { return a > 3 && a < 9; })
		.orderby(DefQuery::sorting_order::ascending)
		.decay();

	// Decay an lvalue -> this is going to make an independent copy of the chain (originalEnumerator) before the decay
	auto copiedEnumerator = originalEnumerator
		.decay();

	// Makes a copy of the decayed chain (copiedEnumerator)
	auto secondCopiedEnumerator = copiedEnumerator;

	// Move the secondCopiedEnumerator to the movedEnumerator, it makes the secondCopiedEnumerator exhausted
	DefQuery::decayed_enumerator<int> movedEnumerator = std::move(secondCopiedEnumerator);

	ASSERT_TRUE(++originalEnumerator);
	ASSERT_EQ(4, *originalEnumerator);
	ASSERT_TRUE(++originalEnumerator);
	ASSERT_EQ(5, *originalEnumerator);
	ASSERT_TRUE(++originalEnumerator);
	ASSERT_EQ(6, *originalEnumerator);
	ASSERT_TRUE(++originalEnumerator);
	ASSERT_EQ(7, *originalEnumerator);
	ASSERT_TRUE(++originalEnumerator);
	ASSERT_EQ(8, *originalEnumerator);
	ASSERT_FALSE(++originalEnumerator);
	ASSERT_FALSE(++originalEnumerator);

	ASSERT_TRUE(++copiedEnumerator);
	ASSERT_EQ(4, *copiedEnumerator);
	ASSERT_TRUE(++copiedEnumerator);
	ASSERT_EQ(5, *copiedEnumerator);
	ASSERT_TRUE(++copiedEnumerator);
	ASSERT_EQ(6, *copiedEnumerator);
	ASSERT_TRUE(++copiedEnumerator);
	ASSERT_EQ(7, *copiedEnumerator);
	ASSERT_TRUE(++copiedEnumerator);
	ASSERT_EQ(8, *copiedEnumerator);
	ASSERT_FALSE(++copiedEnumerator);
	ASSERT_FALSE(++copiedEnumerator);

	ASSERT_TRUE(++movedEnumerator);
	ASSERT_EQ(4, *movedEnumerator);
	ASSERT_TRUE(++movedEnumerator);
	ASSERT_EQ(5, *movedEnumerator);
	ASSERT_TRUE(++movedEnumerator);
	ASSERT_EQ(6, *movedEnumerator);
	ASSERT_TRUE(++movedEnumerator);
	ASSERT_EQ(7, *movedEnumerator);
	ASSERT_TRUE(++movedEnumerator);
	ASSERT_EQ(8, *movedEnumerator);
	ASSERT_FALSE(++movedEnumerator);
	ASSERT_FALSE(++movedEnumerator);

	// As the secondCopiedEnumerator was moved into movedEnumerator, it is immediately exhausted
	ASSERT_FALSE(++secondCopiedEnumerator);
	ASSERT_FALSE(++secondCopiedEnumerator);
}

