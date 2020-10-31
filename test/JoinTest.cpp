#include "gtest/gtest.h"
#include <map>
#include <string>
#include <DefQuery/from.h>
#include <DefQuery/where.h>
#include <DefQuery/join.h>
#include <DefQuery/stlrange.h>

TEST(JoinTest, InnerJoinTest)
{
	std::map<int, std::string> names = { {5, "John"}, {7, "James"}, {11, "Peter"} };
	std::map<int, double> heights = { {7, 1.90}, {5, 1.80}, {11, 1.70} };

	struct PersonData
	{
		std::string _name;
		double _height;
	};

	auto enumerator = DefQuery::from(names)
		.join(DefQuery::from(heights),
			[](const auto& namesEntry) { auto key = namesEntry.first; return key; },
			[](const auto& heightsEntry) { auto key = heightsEntry.first; return key; },
			[](const auto& namesEntry, const auto& heightsEntry) { auto& name = namesEntry.second; auto height = heightsEntry.second; return PersonData{ name, height }; });

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("John", enumerator->_name);
	ASSERT_EQ(1.8, enumerator->_height);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("James", enumerator->_name);
	ASSERT_EQ(1.9, enumerator->_height);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Peter", enumerator->_name);
	ASSERT_EQ(1.7, enumerator->_height);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);
}

TEST(JoinTest, CopyTest)
{
	std::map<int, std::string> names = { {5, "John"}, {7, "James"}, {11, "Peter"} };
	std::map<int, double> heights = { {7, 1.90}, {5, 1.80}, {11, 1.70} };

	struct PersonData
	{
		std::string _name;
		double _height;
	};

	auto enumerator = DefQuery::from(names)
		.join(DefQuery::from(heights),
			[](const auto& namesEntry) { auto key = namesEntry.first; return key; },
			[](const auto& heightsEntry) { auto key = heightsEntry.first; return key; },
			[](const auto& namesEntry, const auto& heightsEntry) { auto& name = namesEntry.second; auto height = heightsEntry.second; return PersonData{ name, height }; });

	auto enumerator2 = enumerator;

	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("John", enumerator->_name);
	ASSERT_EQ(1.8, enumerator->_height);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("James", enumerator->_name);
	ASSERT_EQ(1.9, enumerator->_height);
	ASSERT_TRUE(++enumerator);
	ASSERT_EQ("Peter", enumerator->_name);
	ASSERT_EQ(1.7, enumerator->_height);
	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("John", enumerator2->_name);
	ASSERT_EQ(1.8, enumerator2->_height);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("James", enumerator2->_name);
	ASSERT_EQ(1.9, enumerator2->_height);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Peter", enumerator2->_name);
	ASSERT_EQ(1.7, enumerator2->_height);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);
}

TEST(JoinTest, MoveTest)
{
	std::map<int, std::string> names = { {5, "John"}, {7, "James"}, {11, "Peter"} };
	std::map<int, double> heights = { {7, 1.90}, {5, 1.80}, {11, 1.70} };

	struct PersonData
	{
		std::string _name;
		double _height;
	};

	auto enumerator = DefQuery::from(names)
		.join(DefQuery::from(heights),
			[](const auto& namesEntry) { auto key = namesEntry.first; return key; },
			[](const auto& heightsEntry) { auto key = heightsEntry.first; return key; },
			[](const auto& namesEntry, const auto& heightsEntry) { auto& name = namesEntry.second; auto height = heightsEntry.second; return PersonData{ name, height }; });

	auto enumerator2 = std::move(enumerator);

	ASSERT_FALSE(++enumerator);
	ASSERT_FALSE(++enumerator);

	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("John", enumerator2->_name);
	ASSERT_EQ(1.8, enumerator2->_height);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("James", enumerator2->_name);
	ASSERT_EQ(1.9, enumerator2->_height);
	ASSERT_TRUE(++enumerator2);
	ASSERT_EQ("Peter", enumerator2->_name);
	ASSERT_EQ(1.7, enumerator2->_height);
	ASSERT_FALSE(++enumerator2);
	ASSERT_FALSE(++enumerator2);
}
