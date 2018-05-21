#include "pch.h"

#include "../DefQuery/enumerator.h"
#include "../DefQuery/where.h"

class quadratic_generator : public enumerator<int, quadratic_generator>
{
public:
	quadratic_generator()
		: _currentIndependent(0)
		, _firstMoveNext(true)
	{}

	bool operator++() override
	{
		if (_firstMoveNext)
			_firstMoveNext = false;
		else
			++_currentIndependent;

		_currentQuadratic = _currentIndependent * _currentIndependent;

		return true;
	}

	const value_type& operator*() const override { return _currentQuadratic; }

private:
	int _currentQuadratic;
	int _currentIndependent;
	bool _firstMoveNext;
};

// ==============================================================================================

TEST(QuadraticTest, GeneratorTest)
{
	auto evenQuadraticNumbers = quadratic_generator()
		.where([](int a) { return a % 2 == 0; });

	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(0, *evenQuadraticNumbers);
	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(4, *evenQuadraticNumbers);
	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(16, *evenQuadraticNumbers);
	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(36, *evenQuadraticNumbers);
}
