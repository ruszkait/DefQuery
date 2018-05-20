#include "pch.h"
#include <array>
#include "../DefQuery/from.h"
#include "../DefQuery/where.h"
#include "../DefQuery/share.h"

TEST(ShareFromTest, ShareTest)
{
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	auto enumerator = from(&arr[0], &arr[4])
		.share();

	ASSERT_TRUE(enumerator->moveNext());
	ASSERT_EQ(1, enumerator->current());
	ASSERT_TRUE(enumerator->moveNext());
	ASSERT_EQ(2, enumerator->current());
	ASSERT_TRUE(enumerator->moveNext());
	ASSERT_EQ(3, enumerator->current());
	ASSERT_TRUE(enumerator->moveNext());
	ASSERT_EQ(4, enumerator->current());
	ASSERT_FALSE(enumerator->moveNext());
	ASSERT_FALSE(enumerator->moveNext());
}

TEST(FilterSharedTest, ShareTest)
{
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	auto enumerator = from(&arr[0], &arr[4])
		.share()
		->where([](int a) { return a == 3; });

	ASSERT_TRUE(enumerator.moveNext());
	ASSERT_EQ(3, enumerator.current());
	ASSERT_FALSE(enumerator.moveNext());
	ASSERT_FALSE(enumerator.moveNext());
}

TEST(DoubleSharingTest, ShareTest)
{
	//std::array<int, 10> arr = { 1,2,3,4,5,6 };

	//auto enumerator = from(&arr[0], &arr[4])
	//	.share()
	//	->share()
	//	->where([](int a) { return a == 3; });

	//ASSERT_TRUE(enumerator.moveNext());
	//ASSERT_EQ(3, enumerator.current());
	//ASSERT_FALSE(enumerator.moveNext());
	//ASSERT_FALSE(enumerator.moveNext());
}
