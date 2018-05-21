#include "pch.h"

#include "../DefQuery/enumerator.h"
#include "../DefQuery/where.h"

class quadratic_generator : public enumerator<int, quadratic_generator>
{
public:
	quadratic_generator();

	bool operator++() override;
	const value_type& operator*() const override;

protected:
	enumerator_interface<value_type>* clone() const override;

private:
	int _currentQuadratic;
	int _currentIndependent;
	bool _firstMoveNext;
};

// ==============================================================================================

quadratic_generator::quadratic_generator()
	: _currentIndependent(0)
{
}

bool quadratic_generator::operator++()
{
	if (_firstMoveNext)
		_firstMoveNext = false;
	else
		++_currentIndependent;

	_currentQuadratic = _currentIndependent * _currentIndependent;

	return true;
}

const quadratic_generator::value_type& quadratic_generator::operator*() const
{
	return _currentQuadratic;
}

enumerator_interface<quadratic_generator::value_type>* quadratic_generator::clone() const
{
	return new quadratic_generator(*this);
}


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
