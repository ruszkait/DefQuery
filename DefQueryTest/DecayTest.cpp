#include "gtest/gtest.h"
#include <array>
#include "../DefQuery/from.h"
#include "../DefQuery/where.h"
#include "../DefQuery/decay.h"

TEST(DecayTest, ShareFromTest)
{
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	DefQuery::decayed_enumerator<int> enumerator = DefQuery::from(&arr[0], &arr[4])
		.decay();

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

TEST(DecayTest, FilterSharedTest)
{
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	// At every decay point, the chain goes to the heap, so it makes it easy to pass it further,
	// extend it even when the current stack frame is gone.
	// This opens up the way to real deferred query execution
	auto enumerator = DefQuery::from(&arr[0], &arr[4])
		.decay()
		.where([](const int a) { return a == 3; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(DecayTest, DoubleSharingTest)
{
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	// Decaying along a chain is useful when independent stages add links to the query chain
	auto enumerator = DefQuery::from(&arr[0], &arr[4])
		.decay()
		.decay()
		.where([](const int a) { return a == 3; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}
