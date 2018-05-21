#include "pch.h"
#include <list>
#include "../DefQuery/from.h"
#include "../DefQuery/selectmany.h"

TEST(SelectManyTest, StructSubcontainerTest)
{
	struct Person
	{
		std::string _name;
		double _age;
		std::vector<std::string> _pets;
	};

	std::vector<Person> lis = { 
		Person{ "Oliver", 10 , { "dog", "cat" }},
		Person{ "Hanna", 11, { "fish", "bird" } },
		Person{ "Peter", 20, { "dog", "bird" } }
	};

	auto enumerator = from(lis)
		.selectmany([](const Person& person) { return from(person._pets); });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("dog", *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("cat", *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("fish", *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("bird", *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("dog", *enumerator);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("bird", *enumerator);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}
