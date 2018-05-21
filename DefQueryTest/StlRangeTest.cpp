#include "pch.h"
#include <array>
#include <list>
#include <algorithm>
#include "../DefQuery/from.h"
#include "../DefQuery/stlrange.h"

TEST(ForTest, StlRangeTest)
{
	std::list<int> copiedValues;
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	auto enumerator = from(&arr[0], &arr[4]);

	for (auto a : enumerator.stlrange())
		copiedValues.push_back(a);

	std::list<int> expectedValues{ 1,2,3,4 };
	ASSERT_EQ(expectedValues, copiedValues);
}

TEST(AlgorithmTest, StlRangeTest)
{
	std::list<int> copiedValues;
	std::array<int, 10> arr = { 1,2,3,4,5,6 };

	auto enumerator = from(&arr[0], &arr[4]).stlrange();

	std::copy(enumerator.begin(), enumerator.end(), std::back_inserter(copiedValues));

	std::list<int> expectedValues{ 1,2,3,4 };
	ASSERT_EQ(expectedValues, copiedValues);
}
