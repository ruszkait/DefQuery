#include "gtest/gtest.h"
#include <array>
#include <list>
#include <algorithm>
#include "../DefQuery/from.h"
#include "../DefQuery/stlrange.h"

TEST(StlRangeTest, ForTest)
{
	std::list<int> copiedValues;
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	auto enumerator = DefQuery::from(&arr[0], &arr[4]);

	for (auto a : enumerator.stlrange())
		copiedValues.push_back(a);

	std::list<int> expectedValues{ 1,2,3,4 };
	ASSERT_EQ(expectedValues, copiedValues);
}

TEST(StlRangeTest, AlgorithmTest)
{
	std::list<int> copiedValues;
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	auto enumerator = DefQuery::from(&arr[0], &arr[4])
							.stlrange();

	std::copy(enumerator.begin(), enumerator.end(), std::back_inserter(copiedValues));

	std::list<int> expectedValues{ 1,2,3,4 };
	ASSERT_EQ(expectedValues, copiedValues);
}
