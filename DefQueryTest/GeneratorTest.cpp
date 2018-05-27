#include "pch.h"

#include "../DefQuery/generator.h"
#include "../DefQuery/where.h"

class quadratic_generator : public generator_enumerator<int, quadratic_generator>
{
public:
	quadratic_generator()
		: _currentIndependent(0)
	{}

	quadratic_generator(const quadratic_generator& other) = default;
	quadratic_generator(quadratic_generator&& other) = default;
	quadratic_generator& operator=(const quadratic_generator& other) = default;
	quadratic_generator& operator=(quadratic_generator&& other) = default;

protected:
	bool try_calculate_next(value_type& nextValue) override
	{
		// Calculate current value
		nextValue = _currentIndependent * _currentIndependent;

		// Move to next state
		++_currentIndependent;

		// Infinite sequence, never gets exhausted
		return true;
	}

private:
	int _currentIndependent;
};

// ==============================================================================================

TEST(GeneratorTest, QuadraticTest)
{
	auto evenQuadraticNumbers = quadratic_generator()
		.where([](const int a) { return a % 2 == 0; });

	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(0, *evenQuadraticNumbers);
	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(4, *evenQuadraticNumbers);
	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(16, *evenQuadraticNumbers);
	ASSERT_TRUE(++evenQuadraticNumbers);
	ASSERT_EQ(36, *evenQuadraticNumbers);
}
