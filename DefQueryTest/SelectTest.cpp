#include "pch.h"
#include <list>
#include "../DefQuery/from.h"
#include "../DefQuery/select.h"

TEST(ProjectionTest, SelectTest)
{
	std::list<int> lis = { 1,2,3 };

	auto enumerator = from(lis)
		.select([](int a) { return a * 1.5; });

	ASSERT_TRUE(enumerator.moveNext());
	ASSERT_EQ(1.5, enumerator.current());
	ASSERT_TRUE(enumerator.moveNext());
	ASSERT_EQ(3.0, enumerator.current());
	ASSERT_TRUE(enumerator.moveNext());
	ASSERT_EQ(4.5, enumerator.current());
	ASSERT_FALSE(enumerator.moveNext());
	ASSERT_FALSE(enumerator.moveNext());
}

TEST(StructProjectionTest, SelectTest)
{
	struct Person
	{
		std::string _name;
		double _age;
	};

	std::vector<Person> lis = { Person{ "Oliver", 10 }, Person{ "Hanna", 11 }, Person{ "Peter", 20 } };

	auto enumerator = from(lis)
		.select([](const Person& a) { return a._age; });

	ASSERT_TRUE(enumerator.moveNext());
	ASSERT_EQ(10, enumerator.current());
	ASSERT_TRUE(enumerator.moveNext());
	ASSERT_EQ(11, enumerator.current());
	ASSERT_TRUE(enumerator.moveNext());
	ASSERT_EQ(20, enumerator.current());
	ASSERT_FALSE(enumerator.moveNext());
	ASSERT_FALSE(enumerator.moveNext());
}
