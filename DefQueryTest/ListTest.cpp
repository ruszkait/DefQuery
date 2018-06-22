#include "gtest/gtest.h"
#include <list>
#include <array>
#include "../DefQuery/from.h"
#include "../DefQuery/list.h"

TEST(ListTest, SubCollectionTest)
{
	std::vector<int> vector = { 1,2,3,4 };

	auto subVectorList = DefQuery::from(vector)
		.list();

	ASSERT_EQ(std::list<int>({ 1,2,3,4 }), subVectorList);
}

TEST(ListTest, EmptyCollectionTest)
{
	std::vector<int> vector;

	auto subVectorList = DefQuery::from(vector)
		.list();

	ASSERT_TRUE(subVectorList.empty());
}
