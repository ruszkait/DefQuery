#include "../DefQuery/enumerator.h"

template<typename TValue, typename TDerived>
class generator_enumerator : public enumerator<TValue, TDerived>
{
public:
	generator_enumerator() = default;

	generator_enumerator(const generator_enumerator& other) = default;
	generator_enumerator(generator_enumerator&& other) = default;
	generator_enumerator& operator=(const generator_enumerator& other) = default;
	generator_enumerator& operator=(generator_enumerator&& other) = default;

	bool operator++();
	const value_type& operator*() const;

protected:
	bool move_next() override { return this->operator++(); }
	const value_type& current() const { return this->operator*(); }

	virtual bool try_calculate_next(TValue& nextValue) = 0;

private:
	TValue _current;
};

// ==============================================================================================

template<typename TValue, typename TDerived>
bool generator_enumerator<TValue, TDerived>::operator++()
{
	auto notExhausted = try_calculate_next(_current);
	return notExhausted;
}

template<typename TValue, typename TDerived>
const typename generator_enumerator<TValue, TDerived>::value_type& generator_enumerator<TValue, TDerived>::operator*() const
{
	return _current;
}
