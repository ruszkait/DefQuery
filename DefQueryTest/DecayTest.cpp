#include "gtest/gtest.h"
#include <vector>
#include <list>
#include "../DefQuery/from.h"
#include "../DefQuery/where.h"
#include "../DefQuery/decay.h"

TEST(DecayTest, DecayFromTest)
{
	std::vector<int> vector = { 1,2,3,4 };

	DefQuery::decayed_enumerator<int> enumerator = DefQuery::from(vector)
		.decay();

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(2, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(4, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(DecayTest, FilterDecayedTest)
{
	std::vector<int> vector = { 1,2,3,4,5,6 };

	auto enumerator = DefQuery::from(vector)
		.decay()
		.where([](const int a) { return a == 3; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(DecayTest, DoubleDecayTest)
{
	std::vector<int> vector = { 1,2,3,4,5,6 };

	auto enumerator = DefQuery::from(vector)
		.decay()
		.decay()
		.where([](const int a) { return a == 3; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(DecayTest, CopyTest)
{
	std::list<int> list = { 1,2,3,4,5,6 };

	DefQuery::decayed_enumerator<int> enumerator = DefQuery::from(list)
		.where([](const int a) { return a == 3; })
		.decay();

	DefQuery::decayed_enumerator<int> enumerator2 = enumerator;

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(3, *enumerator2);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);
}

TEST(DecayTest, MoveTest)
{
	std::list<int> list = { 1,2,3,4,5,6 };

	DefQuery::decayed_enumerator<int> enumerator = DefQuery::from(list)
		.where([](const int a) { return a == 3; })
		.decay();

	DefQuery::decayed_enumerator<int> enumerator2 = std::move(enumerator);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(3, *enumerator2);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);
}
