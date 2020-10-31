#include "gtest/gtest.h"
#include <list>
#include <regex>
#include <string>
#include <ctime>
#include <DefQuery/range.h>
#include <DefQuery/where.h>
#include <DefQuery/select.h>
#include <DefQuery/orderby.h>

TEST(RegexpTest, IntegerRegisterRegexpTest)
{
	std::string text = "realValue=#r55# integerValue=#i46# badFormedValue=#55# integerValue=#i59# lastIntegerValue=#i12#";
	std::regex pattern("#([ribs])([0-9]+)#");

	auto integerReferenceNumbersEnumerator = DefQuery::range(std::sregex_iterator(text.begin(), text.end(), pattern), std::sregex_iterator())
		.where([](const auto& matchIter) { auto referenceTypeAsString = (*matchIter)[1]; return referenceTypeAsString == 'i'; })
		.select([](const auto& matchIter) { auto referenceNumberAsString = (*matchIter)[2]; return std::stoi(referenceNumberAsString); })
		.orderby(DefQuery::sorting_order::ascending);

	ASSERT_TRUE(++integerReferenceNumbersEnumerator);
	ASSERT_EQ(12, *integerReferenceNumbersEnumerator);
	ASSERT_TRUE(++integerReferenceNumbersEnumerator);
	ASSERT_EQ(46, *integerReferenceNumbersEnumerator);
	ASSERT_TRUE(++integerReferenceNumbersEnumerator);
	ASSERT_EQ(59, *integerReferenceNumbersEnumerator);
	ASSERT_FALSE(++integerReferenceNumbersEnumerator);
	ASSERT_FALSE(++integerReferenceNumbersEnumerator);
}

TEST(RegexpTest, DateTimeRegexpTest)
{
	std::string text = "15:19:00 17/06/2018, 14:19:00 16/06/2016, 14:19:00";
	std::regex pattern("(([0-9]+):([0-9]+):([0-9]+)) (([0-9]+)/([0-9]+)/([0-9]+))");

	auto dateTimeEnumerator = DefQuery::range(std::sregex_iterator(text.begin(), text.end(), pattern), std::sregex_iterator())
		.select([](auto& matchIter)
	{
		std::tm timeStruct;

		// index 0: the full match, like: 15:19:00 17/06/2018
		// further indices are the count of the starting parenthesises

		timeStruct.tm_hour = std::stoi((*matchIter)[2]);
		timeStruct.tm_min = std::stoi((*matchIter)[3]);
		timeStruct.tm_sec = std::stoi((*matchIter)[4]);

		timeStruct.tm_mday = std::stoi((*matchIter)[6]);
		timeStruct.tm_mon = std::stoi((*matchIter)[7]) - 1;
		timeStruct.tm_year = std::stoi((*matchIter)[8]) - 1900;

		auto timeSinceEpoch = std::mktime(&timeStruct);
		return timeSinceEpoch;
	})
		.orderby(DefQuery::sorting_order::ascending)
		.select([](auto& timeSinceEpoch)
	{
		auto timeStruct = *std::localtime(&timeSinceEpoch);
		return std::string(std::asctime(&timeStruct));
	});

	ASSERT_TRUE(++dateTimeEnumerator);
	ASSERT_EQ("Thu Jun 16 14:19:00 2016\n", *dateTimeEnumerator);
	ASSERT_TRUE(++dateTimeEnumerator);
	ASSERT_EQ("Sun Jun 17 15:19:00 2018\n", *dateTimeEnumerator);
	ASSERT_FALSE(++dateTimeEnumerator);
	ASSERT_FALSE(++dateTimeEnumerator);
}

