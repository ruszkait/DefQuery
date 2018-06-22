#include "gtest/gtest.h"
#include <list>
#include "../DefQuery/from.h"
#include "../DefQuery/orderby.h"

TEST(OrderByTest, MultiStageSortingTest)
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
	   Person{ "Hanna", 11, { "fish", "bird" } },
	   Person{ "Hanna", 11, { "fish" } },
	   Person{ "Hanna", 14, { "fish", "bird" } },
	   Person{ "Peter", 20, { "dog", "bird" } }
	};


	auto enumerator = DefQuery::from(persons)
		.orderby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._name.compare(personB._name); })
		.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return int(personA._age - personB._age); })
		.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._pets.size() < personB._pets.size() ? -1 : personA._pets.size() == personB._pets.size() ? 0 : +1; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(11, enumerator->_age);
	ASSERT_EQ(1, enumerator->_pets.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(11, enumerator->_age);
	ASSERT_EQ(2, enumerator->_pets.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(14, enumerator->_age);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(19, enumerator->_age);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Oliver", enumerator->_name);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Peter", enumerator->_name);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(OrderByTest, DefaultComparatorTest)
{
	std::vector<std::string> names = { "Hanna", "Oliver", "Hanna", "Peter" };

	auto enumerator = DefQuery::from(names)
		.orderby(DefQuery::sorting_order::ascending);

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Oliver", *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Peter", *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(OrderByTest, EmptyTest)
{
	struct Person
	{
		std::string _name;
		double _age;
		std::vector<std::string> _pets;
	};

	std::vector<Person> persons;

	auto enumerator = DefQuery::from(persons)
		.orderby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._name.compare(personB._name); })
		.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return int(personA._age - personB._age); });

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(OrderByTest, DescendingOrderTest)
{
	struct Person
	{
		std::string _name;
		double _age;
	};

	std::vector<Person> persons = {
	   Person{ "Hanna", 9 },
	   Person{ "Oliver", 10 },
	   Person{ "Hanna", 19 },
	   Person{ "Peter", 20 }
	};

	auto enumerator = DefQuery::from(persons)
		.orderby(DefQuery::sorting_order::descending, [](const auto& personA, const auto& personB) { return personA._name.compare(personB._name); })
		.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return int(personA._age - personB._age); });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Peter", enumerator->_name);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Oliver", enumerator->_name);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(9, enumerator->_age);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(19, enumerator->_age);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(OrderByTest, CopyTest)
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
	   Person{ "Hanna", 11, { "fish", "bird" } },
	   Person{ "Hanna", 11, { "fish" } },
	   Person{ "Hanna", 14, { "fish", "bird" } },
	   Person{ "Peter", 20, { "dog", "bird" } }
	};


	auto enumerator = DefQuery::from(persons)
		.orderby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._name.compare(personB._name); })
		.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return int(personA._age - personB._age); })
		.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._pets.size() < personB._pets.size() ? -1 : personA._pets.size() == personB._pets.size() ? 0 : +1; });

	auto enumerator2 = enumerator;

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(11, enumerator->_age);
	ASSERT_EQ(1, enumerator->_pets.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(11, enumerator->_age);
	ASSERT_EQ(2, enumerator->_pets.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(14, enumerator->_age);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(19, enumerator->_age);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Oliver", enumerator->_name);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Peter", enumerator->_name);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);


	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(11, enumerator2->_age);
	ASSERT_EQ(1, enumerator2->_pets.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(11, enumerator2->_age);
	ASSERT_EQ(2, enumerator2->_pets.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(14, enumerator2->_age);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(19, enumerator2->_age);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Oliver", enumerator2->_name);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Peter", enumerator2->_name);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);
}

TEST(OrderByTest, CopyStartedTest)
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
	   Person{ "Hanna", 11, { "fish", "bird" } },
	   Person{ "Hanna", 11, { "fish" } },
	   Person{ "Hanna", 14, { "fish", "bird" } },
	   Person{ "Peter", 20, { "dog", "bird" } }
	};


	auto enumerator = DefQuery::from(persons)
		.orderby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._name.compare(personB._name); })
		.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return int(personA._age - personB._age); })
		.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._pets.size() < personB._pets.size() ? -1 : personA._pets.size() == personB._pets.size() ? 0 : +1; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(11, enumerator->_age);
	ASSERT_EQ(1, enumerator->_pets.size());


	auto enumerator2 = enumerator;

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(11, enumerator->_age);
	ASSERT_EQ(2, enumerator->_pets.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(14, enumerator->_age);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(19, enumerator->_age);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Oliver", enumerator->_name);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Peter", enumerator->_name);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);


	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(11, enumerator2->_age);
	ASSERT_EQ(2, enumerator2->_pets.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(14, enumerator2->_age);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(19, enumerator2->_age);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Oliver", enumerator2->_name);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Peter", enumerator2->_name);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);
}

TEST(OrderByTest, MoveTest)
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
	   Person{ "Hanna", 11, { "fish", "bird" } },
	   Person{ "Hanna", 11, { "fish" } },
	   Person{ "Hanna", 14, { "fish", "bird" } },
	   Person{ "Peter", 20, { "dog", "bird" } }
	};


	auto enumerator = DefQuery::from(persons)
		.orderby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._name.compare(personB._name); })
		.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return int(personA._age - personB._age); })
		.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._pets.size() < personB._pets.size() ? -1 : personA._pets.size() == personB._pets.size() ? 0 : +1; });

	auto enumerator2 = std::move(enumerator);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);


	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(11, enumerator2->_age);
	ASSERT_EQ(1, enumerator2->_pets.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(11, enumerator2->_age);
	ASSERT_EQ(2, enumerator2->_pets.size());
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(14, enumerator2->_age);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(19, enumerator2->_age);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Oliver", enumerator2->_name);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Peter", enumerator2->_name);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);
}

TEST(OrderByTest, MoveStartedTest)
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
	   Person{ "Hanna", 11, { "fish", "bird" } },
	   Person{ "Hanna", 11, { "fish" } },
	   Person{ "Hanna", 14, { "fish", "bird" } },
	   Person{ "Peter", 20, { "dog", "bird" } }
	};

	auto enumerator = DefQuery::from(persons)
		.orderby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._name.compare(personB._name); })
		.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return int(personA._age - personB._age); })
		.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._pets.size() < personB._pets.size() ? -1 : personA._pets.size() == personB._pets.size() ? 0 : +1; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(11, enumerator->_age);
	ASSERT_EQ(1, enumerator->_pets.size());
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Hanna", enumerator->_name);
	ASSERT_EQ(11, enumerator->_age);
	ASSERT_EQ(2, enumerator->_pets.size());

	auto enumerator2 = std::move(enumerator);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(14, enumerator2->_age);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Hanna", enumerator2->_name);
	ASSERT_EQ(19, enumerator2->_age);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Oliver", enumerator2->_name);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Peter", enumerator2->_name);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);
}
