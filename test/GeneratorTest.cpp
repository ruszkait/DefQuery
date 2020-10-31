#include "gtest/gtest.h"
#include <DefQuery/generator.h>
#include <DefQuery/where.h>

TEST(GeneratorTest, QuadraticTest)
{
	auto evenQuadraticNumbers = DefQuery::generator<int>([_currentIndependent = 0](int& nextValue) mutable
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

TEST(GeneratorTest, CopyTest)
{
	auto evenQuadraticNumbers = DefQuery::generator<int>([_currentIndependent = 0](int& nextValue) mutable
	{
		// Calculate current value
		nextValue = _currentIndependent * _currentIndependent;

		// Move to next state
		++_currentIndependent;

		// Infinite sequence, never gets exhausted
		return true;
	});

	auto evenQuadraticNumbersCopy = evenQuadraticNumbers;

	ASSERT_TRUE(++evenQuadraticNumbersCopy);
	ASSERT_EQ(0, *evenQuadraticNumbersCopy);
	ASSERT_TRUE(++evenQuadraticNumbersCopy);
	ASSERT_EQ(1, *evenQuadraticNumbersCopy);
	ASSERT_TRUE(++evenQuadraticNumbersCopy);
	ASSERT_EQ(4, *evenQuadraticNumbersCopy);
	ASSERT_TRUE(++evenQuadraticNumbersCopy);
	ASSERT_EQ(9, *evenQuadraticNumbersCopy);

	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(0, *evenQuadraticNumbers);
	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(1, *evenQuadraticNumbers);
	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(4, *evenQuadraticNumbers);
	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(9, *evenQuadraticNumbers);
}

TEST(GeneratorTest, MoveTest)
{
	auto evenQuadraticNumbers = DefQuery::generator<int>([_currentIndependent = 0](int& nextValue) mutable
	{
		// Calculate current value
		nextValue = _currentIndependent * _currentIndependent;

		// Move to next state
		++_currentIndependent;

		// Infinite sequence, never gets exhausted
		return true;
	});

	auto evenQuadraticNumbers2 = std::move(evenQuadraticNumbers);

	ASSERT_TRUE(++evenQuadraticNumbers2);
	ASSERT_EQ(0, *evenQuadraticNumbers2);
	ASSERT_TRUE(++evenQuadraticNumbers2);
	ASSERT_EQ(1, *evenQuadraticNumbers2);
	ASSERT_TRUE(++evenQuadraticNumbers2);
	ASSERT_EQ(4, *evenQuadraticNumbers2);
	ASSERT_TRUE(++evenQuadraticNumbers2);
	ASSERT_EQ(9, *evenQuadraticNumbers2);

	ASSERT_FALSE(++evenQuadraticNumbers);
	ASSERT_FALSE(++evenQuadraticNumbers);
}
