#include "gtest/gtest.h"
#include <list>
#include <string>
#include <sstream>
#include "../DefQuery/from.h"
#include "../DefQuery/aggregate.h"

TEST(AggregateTest, SumIntegersTest)
{
	std::list<int> list = { 1,2,3 };

	auto sum = DefQuery::from(list)
		.aggregate([](double& accumlator, int a) { accumlator += a; },
			[](int a) { return static_cast<double>(a); });

	ASSERT_EQ(6.0, sum);
}

TEST(AggregateTest, JoinStringsTest)
{
	std::list<int> list = { 1,2,3 };

	auto joinedList = DefQuery::from(list)
		.aggregate([](std::string& accumlator, int a) { accumlator += ',' + std::to_string(a); },
			[](int a) { return std::to_string(a); });

	ASSERT_EQ("1,2,3", joinedList);
}

TEST(AggregateTest, JoinStringsToStreamTest)
{
	std::list<int> list = { 1,2,3 };

	auto joinedList = DefQuery::from(list)
		.aggregate([](std::stringstream& accumlator, int a) { accumlator << ',' << a; },
			[](int a) { std::stringstream accumlator; accumlator << a; return accumlator; });

	ASSERT_EQ("1,2,3", joinedList.str());
}

TEST(AggregateTest, JoinStringsToExistingStreamTest)
{
	std::list<int> list = { 1,2,3 };

	std::stringstream stringStream;
	stringStream << "my numbers:";

	// Preferably do not use the return value in this case, just check the captured stringStream content
	// The return value is a pointer to the stringStream or a nullptr if the input enumerator was empty
	DefQuery::from(list)
		.aggregate([](std::stringstream* accumlator, int a) { *accumlator << ',' << a; },
			[stringStream = &stringStream](int a) { *stringStream << a; return stringStream; });

	ASSERT_EQ("my numbers:1,2,3", stringStream.str());
}

TEST(AggregateTest, AggregateOneTest)
{
	std::list<int> list = { 1 };

	auto joinedList = DefQuery::from(list)
		.aggregate([](std::string& accumlator, int a) { accumlator += ',' + std::to_string(a); },
			[](int a) { return std::to_string(a); });

	ASSERT_EQ("1", joinedList);
}

TEST(AggregateTest, AggregateNoneTest)
{
	std::list<int> list;

	EXPECT_THROW(DefQuery::from(list)
		.aggregate([](std::string& accumlator, int a) { accumlator += ',' + std::to_string(a); },
			[](int a) { return std::to_string(a); }), std::runtime_error);
}
