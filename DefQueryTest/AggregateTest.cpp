#include "pch.h"
#include <list>
#include <string>
#include <sstream>
#include "../DefQuery/from.h"
#include "../DefQuery/aggregate.h"

TEST(AggregateTest, SumIntegersTest)
{
	std::list<int> lis = { 1,2,3 };

	auto sum = DefQuery::from(lis)
		.aggregate([](double& accumlator, int a) { accumlator += a; },
			[](int a) { return static_cast<double>(a); });

	ASSERT_EQ(6.0, sum);
}

TEST(AggregateTest, JoinStringsTest)
{
	std::list<int> lis = { 1,2,3 };

	auto joinedList = DefQuery::from(lis)
		.aggregate([](std::string& accumlator, int a) { accumlator += ',' + std::to_string(a); },
			[](int a) { return std::to_string(a); });

	ASSERT_EQ("1,2,3", joinedList);
}
