#include "gtest/gtest.h"
#include <DefQuery/from.h>
#include <DefQuery/vector.h>
#include <list>
#include <vector>

TEST(VectorTest, SubCollectionTest)
{
	std::vector<int> vector = {1, 2, 3, 4};

	auto subVector = DefQuery::from(vector).vector();

	ASSERT_EQ(std::vector<int>({1, 2, 3, 4}), subVector);
}

TEST(VectorTest, EmptyCollectionTest)
{
	std::vector<int> vector;

	auto subVector = DefQuery::from(vector).vector();

	ASSERT_TRUE(subVector.empty());
}
