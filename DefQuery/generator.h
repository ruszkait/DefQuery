#pragma once 

#include "../DefQuery/enumerator.h"
#include <functional>

namespace DefQuery
{
	template<typename TValue>
	class generator_enumerator : public enumerator<TValue, generator_enumerator<TValue>>
	{
	public:
		generator_enumerator(const std::function<bool(TValue&)>& yieldCalculator);

		generator_enumerator(const generator_enumerator& other) = default;
		generator_enumerator(generator_enumerator&& other) = default;
		generator_enumerator& operator=(const generator_enumerator& other) = default;
		generator_enumerator& operator=(generator_enumerator&& other) = default;

		bool operator++();
		const TValue& operator*() const;

	protected:
		bool move_next() override { return this->operator++(); }
		const TValue& current() const override { return this->operator*(); }

	private:
		std::function<bool(TValue&)> _yieldCalculator;
		TValue _current;
	};

	// ==============================================================================================

	template<typename TValue>
	generator_enumerator<TValue> generator(const std::function<bool(TValue&)>& yieldCalculator)
	{
		return generator_enumerator<TValue>(yieldCalculator);
	}

	template<typename TValue>
	generator_enumerator<TValue>::generator_enumerator(const std::function<bool(TValue&)>& yieldCalculator)
		: _yieldCalculator(yieldCalculator)
	{}

	template<typename TValue>
	bool generator_enumerator<TValue>::operator++()
	{
		auto notExhausted = _yieldCalculator(_current);
		return notExhausted;
	}

	template<typename TValue>
	const TValue& generator_enumerator<TValue>::operator*() const
	{
		return _current;
	}
}
