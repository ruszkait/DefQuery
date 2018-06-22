#include "gtest/gtest.h"
#include <list>
#include <vector>
#include <map>
#include "../DefQuery/from.h"
#include "../DefQuery/store.h"

TEST(StoreTest, ListTest)
{
	std::vector<int> vector = { 1,2,3,4 };

	auto convertedContainer = DefQuery::from(vector)
		.store<std::list<int>>([](auto& list, auto number) { list.emplace_back(number); });

	ASSERT_EQ(std::list<int>({ 1,2,3,4 }), convertedContainer);
}

TEST(StoreTest, VectorTest)
{
	std::list<int> list = { 1,2,3,4 };

	auto convertedContainer = DefQuery::from(list)
		.store<std::vector<int>>([](auto& vector, auto number) { vector.emplace_back(number); });

	ASSERT_EQ(std::vector<int>({ 1,2,3,4 }), convertedContainer);
}

TEST(StoreTest, MapTest)
{
	struct Person
	{
		std::string _name;
		double _age;
		std::vector<std::string> _pets;
	};

	std::vector<Person> persons = {
	   Person{ "Hanna", 9, { "fish", "bird" } },
	   Person{ "Oliver", 10 , { "dog", "cat" }},
	   Person{ "Eve", 19, { "fish", "bird" } },
	   Person{ "Peter", 20, { "dog", "bird" } }
	};

	auto convertedContainer = DefQuery::from(persons)
		.store<std::map<std::string, double>>([](auto& map, auto& person) { map.emplace(person._name, person._age); });

	std::map<std::string, double> expectedResult{ {"Hanna",9.0}, {"Oliver",10.0}, {"Eve",19.0}, {"Peter",20.0} };

	ASSERT_EQ(expectedResult, convertedContainer);
}
