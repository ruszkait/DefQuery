#include "gtest/gtest.h"
#include <list>
#include <DefQuery/from.h>
#include <DefQuery/groupby.h>

TEST(GroupByTest, EvenOddNumbersTest)
{
	std::list<int> list = { 1,8,7,11 };

	auto enumerator = DefQuery::from(list)
		.groupby([](int a) { return a % 2 == 0; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(false, enumerator->_key);
	std::vector<int> oddNumbers = { 1,7,11 };
	ASSERT_EQ(oddNumbers, enumerator->_members);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ(true, enumerator->_key);
	std::vector<int> evenNumbers = { 8 };
	ASSERT_EQ(evenNumbers, enumerator->_members);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(GroupByTest, NameGroupingTest)
{
	struct Person
	{
		std::string _name;
		double _age;
		std::vector<std::string> _pets;
	};

	std::vector<Person> persons = {
	   Person{ "Oliver", 10 , { "dog", "cat" }},
	   Person{ "Hanna", 19, { "fish", "bird" } },
	   Person{ "Arnold", 11, { "fish", "bird" } },
	   Person{ "Henrik", 19, { "fish", "bird" } },
	   Person{ "Peter", 20, { "dog", "bird" } },
	   Person{ "Harald", 19, { "fish", "bird" } },
	   Person{ "Agnes", 15, { "cat", "bird" } }
	};

	// Group by the first letter of the name
	auto enumerator = DefQuery::from(persons)
		.groupby([](const auto& person) { return person._name[0]; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('O', enumerator->_key);
	ASSERT_EQ(1, enumerator->_members.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('H', enumerator->_key);
	ASSERT_EQ(3, enumerator->_members.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('A', enumerator->_key);
	ASSERT_EQ(2, enumerator->_members.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('P', enumerator->_key);
	ASSERT_EQ(1, enumerator->_members.size());
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(GroupByTest, CopyTest)
{
	struct Person
	{
		std::string _name;
		double _age;
		std::vector<std::string> _pets;
	};

	std::vector<Person> persons = {
	   Person{ "Oliver", 10 , { "dog", "cat" }},
	   Person{ "Hanna", 19, { "fish", "bird" } },
	   Person{ "Arnold", 11, { "fish", "bird" } },
	   Person{ "Henrik", 19, { "fish", "bird" } },
	   Person{ "Peter", 20, { "dog", "bird" } },
	   Person{ "Harald", 19, { "fish", "bird" } },
	   Person{ "Agnes", 15, { "cat", "bird" } }
	};

	auto enumerator = DefQuery::from(persons)
		.groupby([](const auto& person) { return person._name[0]; });

	auto enumerator2 = enumerator;

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('O', enumerator2->_key);
	ASSERT_EQ(1, enumerator2->_members.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('H', enumerator2->_key);
	ASSERT_EQ(3, enumerator2->_members.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('A', enumerator2->_key);
	ASSERT_EQ(2, enumerator2->_members.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('P', enumerator2->_key);
	ASSERT_EQ(1, enumerator2->_members.size());
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('O', enumerator->_key);
	ASSERT_EQ(1, enumerator->_members.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('H', enumerator->_key);
	ASSERT_EQ(3, enumerator->_members.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('A', enumerator->_key);
	ASSERT_EQ(2, enumerator->_members.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('P', enumerator->_key);
	ASSERT_EQ(1, enumerator->_members.size());
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(GroupByTest, MoveTest)
{
	struct Person
	{
		std::string _name;
		double _age;
		std::vector<std::string> _pets;
	};

	std::vector<Person> persons = {
	   Person{ "Oliver", 10 , { "dog", "cat" }},
	   Person{ "Hanna", 19, { "fish", "bird" } },
	   Person{ "Arnold", 11, { "fish", "bird" } },
	   Person{ "Henrik", 19, { "fish", "bird" } },
	   Person{ "Peter", 20, { "dog", "bird" } },
	   Person{ "Harald", 19, { "fish", "bird" } },
	   Person{ "Agnes", 15, { "cat", "bird" } }
	};

	auto enumerator = DefQuery::from(persons)
		.groupby([](const auto& person) { return person._name[0]; });

	auto enumerator2 = std::move(enumerator);

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('O', enumerator2->_key);
	ASSERT_EQ(1, enumerator2->_members.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('H', enumerator2->_key);
	ASSERT_EQ(3, enumerator2->_members.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('A', enumerator2->_key);
	ASSERT_EQ(2, enumerator2->_members.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('P', enumerator2->_key);
	ASSERT_EQ(1, enumerator2->_members.size());
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(GroupByTest, CopyStartedTest)
{
	struct Person
	{
		std::string _name;
		double _age;
		std::vector<std::string> _pets;
	};

	std::vector<Person> persons = {
	   Person{ "Oliver", 10 , { "dog", "cat" }},
	   Person{ "Hanna", 19, { "fish", "bird" } },
	   Person{ "Arnold", 11, { "fish", "bird" } },
	   Person{ "Henrik", 19, { "fish", "bird" } },
	   Person{ "Peter", 20, { "dog", "bird" } },
	   Person{ "Harald", 19, { "fish", "bird" } },
	   Person{ "Agnes", 15, { "cat", "bird" } }
	};

	auto enumerator = DefQuery::from(persons)
		.groupby([](const auto& person) { return person._name[0]; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('O', enumerator->_key);
	ASSERT_EQ(1, enumerator->_members.size());

	auto enumerator2 = enumerator;

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('H', enumerator2->_key);
	ASSERT_EQ(3, enumerator2->_members.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('A', enumerator2->_key);
	ASSERT_EQ(2, enumerator2->_members.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('P', enumerator2->_key);
	ASSERT_EQ(1, enumerator2->_members.size());
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('H', enumerator->_key);
	ASSERT_EQ(3, enumerator->_members.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('A', enumerator->_key);
	ASSERT_EQ(2, enumerator->_members.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('P', enumerator->_key);
	ASSERT_EQ(1, enumerator->_members.size());
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(GroupByTest, MoveStartedTest)
{
	struct Person
	{
		std::string _name;
		double _age;
		std::vector<std::string> _pets;
	};

	std::vector<Person> persons = {
	   Person{ "Oliver", 10 , { "dog", "cat" }},
	   Person{ "Hanna", 19, { "fish", "bird" } },
	   Person{ "Arnold", 11, { "fish", "bird" } },
	   Person{ "Henrik", 19, { "fish", "bird" } },
	   Person{ "Peter", 20, { "dog", "bird" } },
	   Person{ "Harald", 19, { "fish", "bird" } },
	   Person{ "Agnes", 15, { "cat", "bird" } }
	};

	auto enumerator = DefQuery::from(persons)
		.groupby([](const auto& person) { return person._name[0]; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ('O', enumerator->_key);
	ASSERT_EQ(1, enumerator->_members.size());

	auto enumerator2 = std::move(enumerator);

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('H', enumerator2->_key);
	ASSERT_EQ(3, enumerator2->_members.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('A', enumerator2->_key);
	ASSERT_EQ(2, enumerator2->_members.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ('P', enumerator2->_key);
	ASSERT_EQ(1, enumerator2->_members.size());
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}
