#include "gtest/gtest.h"
#include <DefQuery/count.h>
#include <DefQuery/from.h>
#include <list>
#include <vector>

TEST(CountTest, SizeOfSubCollectionTest)
{
	std::vector<int> vector = {1, 2, 3, 4};

	auto subArrSize = DefQuery::from(vector).count();

	ASSERT_EQ(4, subArrSize);
}

TEST(CountTest, SizeOfCollectionTest)
{
	std::list<int> collection = {1, 2, 3, 4, 5, 6};

	auto collectionSize = DefQuery::from(collection).count();

	ASSERT_EQ(6, collectionSize);
}
