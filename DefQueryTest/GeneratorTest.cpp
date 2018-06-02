#include "gtest/gtest.h"
#include "../DefQuery/generator.h"
#include "../DefQuery/where.h"

TEST(GeneratorTest, QuadraticTest)
{
	auto evenQuadraticNumbers = DefQuery::generator<int>([_currentIndependent = 0] (int& nextValue) mutable
		{
			// Calculate current value
			nextValue = _currentIndependent * _currentIndependent;

			// Move to next state
			++_currentIndependent;

			// Infinite sequence, never gets exhausted
			return true;
		})
		.where([](const int a) { return a % 2 == 0; });

	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(0, *evenQuadraticNumbers);
	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(4, *evenQuadraticNumbers);
	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(16, *evenQuadraticNumbers);
	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(36, *evenQuadraticNumbers);
}
