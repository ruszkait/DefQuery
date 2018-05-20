#include "pch.h"
#include <list>
#include "../DefQuery/from.h"
#include "../DefQuery/where.h"

TEST(TestCaseName, TestName)
{
	std::list<int> lis = { 1,2,3,4,5,6 };

	auto asd = from(lis);
	auto asd2 = from(lis).where([](int a) { return a < 4; });
	//	auto asd = from(lis)>>where([](int a) { return a < 4; });

	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}
