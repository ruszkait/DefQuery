#include <DefQuery/orderby.h>
#include <DefQuery/range.h>
#include <DefQuery/select.h>
#include <DefQuery/where.h>
#include <ctime>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <list>
#include <regex>
#include <string>

using namespace ::testing;
TEST(RegexpTest, IntegerRegisterRegexpTest)
{
	std::string text = "realValue=#r55# integerValue=#i46# badFormedValue=#55# integerValue=#i59# "
					   "lastIntegerValue=#i12#";
	std::regex pattern("#([ribs])([0-9]+)#");

	auto integerReferenceNumbersEnumerator =
		DefQuery::range(std::sregex_iterator(text.begin(), text.end(), pattern),
						std::sregex_iterator())
			.where([](const auto& matchIter) {
				auto referenceTypeAsString = (*matchIter)[1];
				return referenceTypeAsString == 'i';
			})
			.select([](const auto& matchIter) {
				auto referenceNumberAsString = (*matchIter)[2];
				return std::stoi(referenceNumberAsString);
			})
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

TEST(RegexpTest, ComplexDataRegexpTest)
{
	std::string text = "Peter 2000, Mary 1995, John 2010";
	std::regex pattern("([[:alpha:]]+) ([[:digit:]]+)");

	struct Person
	{
		std::string name;
		std::size_t birth_year;
	};

	auto dateTimeEnumerator =
		DefQuery::range(std::sregex_iterator(text.begin(), text.end(), pattern),
						std::sregex_iterator())
			.select([](auto& matchIter) {
				Person person;
				person.name = (*matchIter)[1];
				person.birth_year = std::stoi((*matchIter)[2]);
				return person;
			})
			.orderby(DefQuery::sorting_order::ascending,
					 [](const Person& a, const Person& b) { return a.birth_year > b.birth_year; })
			.select([](const Person& person) {
				std::stringstream personTextStream;
				personTextStream << person.name << " was born in " << person.birth_year;

				return personTextStream.str();
			});

	ASSERT_TRUE(++dateTimeEnumerator);
	ASSERT_THAT(*dateTimeEnumerator, StrEq("Mary was born in 1995"));
	ASSERT_TRUE(++dateTimeEnumerator);
	ASSERT_THAT(*dateTimeEnumerator, StrEq("Peter was born in 2000"));
	ASSERT_TRUE(++dateTimeEnumerator);
	ASSERT_THAT(*dateTimeEnumerator, StrEq("John was born in 2010"));
	ASSERT_FALSE(++dateTimeEnumerator);
	ASSERT_FALSE(++dateTimeEnumerator);
}
