#include "gtest/gtest.h"
#include <list>
#include "../DefQuery/from.h"
#include "../DefQuery/select.h"

TEST(SelectTest, ProjectionTest)
{
	std::list<int> list = { 1,2,3 };

	auto enumerator = DefQuery::from(list)
		.select([](int a) { return a * 1.5; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(1.5, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(3.0, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(4.5, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(SelectTest, StructProjectionTest)
{
	struct Person
	{
		std::string _name;
		double _age;
	};

	std::vector<Person> list = { Person{ "Oliver", 10 }, Person{ "Hanna", 11 }, Person{ "Peter", 20 } };

	auto enumerator = DefQuery::from(list)
		.select([](const Person& person) { return person._age; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(10, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(11, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(20, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(SelectTest, CopyTest)
{
	struct Person
	{
		std::string _name;
		double _age;
	};

	std::vector<Person> list = { Person{ "Oliver", 10 }, Person{ "Hanna", 11 }, Person{ "Peter", 20 } };

	auto enumerator = DefQuery::from(list)
		.select([](const Person& person) { return person._age; });

	auto enumeratorCopy = enumerator;

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(10, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(11, *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(20, *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);

	ASSERT_TRUE(++enumeratorCopy);
	ASSERT_EQ(10, *enumeratorCopy);
	ASSERT_TRUE(++enumeratorCopy);
	ASSERT_EQ(11, *enumeratorCopy);
	ASSERT_TRUE(++enumeratorCopy);
	ASSERT_EQ(20, *enumeratorCopy);
	ASSERT_FALSE(++enumeratorCopy);
	ASSERT_FALSE(++enumeratorCopy);
}

TEST(SelectTest, MoveTest)
{
	struct Person
	{
		std::string _name;
		double _age;
	};

	std::vector<Person> lis = { Person{ "Oliver", 10 }, Person{ "Hanna", 11 }, Person{ "Peter", 20 } };

	auto enumerator = DefQuery::from(lis)
		.select([](const Person& person) { return person._age; });

	auto enumerator2 = std::move(enumerator);

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(10, *enumerator2);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(11, *enumerator2);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ(20, *enumerator2);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}
