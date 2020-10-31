#include "gtest/gtest.h"
#include <vector>
#include <list>
#include <algorithm>
#include <DefQuery/from.h>
#include <DefQuery/stlrange.h>

TEST(StlRangeTest, EmptyTest)
{
	std::list<int> emptyList;

	auto enumerator = DefQuery::from(emptyList);

	for (auto a : enumerator.stlrange())
		ASSERT_TRUE(false);
}

TEST(StlRangeTest, ForTest)
{
	std::list<int> copiedValues;
	std::vector<int> vector = { 1,2,3,4 };

	auto enumerator = DefQuery::from(vector);

	for (auto a : enumerator.stlrange())
		copiedValues.push_back(a);

	std::list<int> expectedValues{ 1,2,3,4 };
	ASSERT_EQ(expectedValues, copiedValues);
}

TEST(StlRangeTest, AlgorithmTest)
{
	std::list<int> copiedValues;
	std::vector<int> vector = { 1,2,3,4 };

	auto enumerator = DefQuery::from(vector)
		.stlrange();

	std::copy(enumerator.begin(), enumerator.end(), std::back_inserter(copiedValues));

	std::list<int> expectedValues{ 1,2,3,4 };
	ASSERT_EQ(expectedValues, copiedValues);
}

