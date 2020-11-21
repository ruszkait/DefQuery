#include "gtest/gtest.h"
#include <DefQuery/from.h>
#include <DefQuery/list.h>
#include <array>
#include <list>

TEST(ListTest, SubCollectionTest)
{
	std::vector<int> vector = {1, 2, 3, 4};

	auto subVectorList = DefQuery::from(vector).list();

	ASSERT_EQ(std::list<int>({1, 2, 3, 4}), subVectorList);
}

TEST(ListTest, EmptyCollectionTest)
{
	std::vector<int> vector;

	auto subVectorList = DefQuery::from(vector).list();

	ASSERT_TRUE(subVectorList.empty());
}
