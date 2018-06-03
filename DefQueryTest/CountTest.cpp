#include "gtest/gtest.h"
#include <list>
#include <array>
#include "../DefQuery/from.h"
#include "../DefQuery/count.h"

TEST(CountTest, SizeOfSubCollectionTest)
{
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	auto subArrSize = DefQuery::from(&arr[0], &arr[4])
		.count();

	ASSERT_EQ(4, subArrSize);
}

TEST(CountTest, SizeOfCollectionTest)
{
	std::list<int> collection = { 1,2,3,4,5,6 };

	auto collectionSize = DefQuery::from(collection)
		.count();

	ASSERT_EQ(6, collectionSize);
}

