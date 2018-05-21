#include "pch.h"
#include <array>
#include "../DefQuery/from.h"
#include "../DefQuery/where.h"
#include "../DefQuery/share.h"

TEST(ShareFromTest, ShareTest)
{
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	shared_enumerator<int> enumerator = from(&arr[0], &arr[4])
		.share();

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

TEST(FilterSharedTest, ShareTest)
{
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	// At every sharing point the part before chain goes to the heap, so it makes it easy to pass it further
	// extend it even when the current stack frame is gone.
	// This opens up the way to real deferred query execution
	auto enumerator = from(&arr[0], &arr[4])
		.share()
		.where([](int a) { return a == 3; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(DoubleSharingTest, ShareTest)
{
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	// Sharing along a chain is useful when independent stages add links to the query chain
	auto enumerator = from(&arr[0], &arr[4])
		.share()
		.share()
		.where([](int a) { return a == 3; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}
