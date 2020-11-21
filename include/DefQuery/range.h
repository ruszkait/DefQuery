#pragma once

#include "enumerator.h"
#include <iterator>

namespace DefQuery
{
template <typename TValue>
class range_enumerator : public enumerator<TValue, range_enumerator<TValue>>
{
public:
	using TBaseClass = enumerator<TValue, range_enumerator<TValue>>;

	range_enumerator();

	range_enumerator(TValue begin, TValue end);

	range_enumerator(const range_enumerator& other) = default;
	range_enumerator(range_enumerator&& other) = default;
	range_enumerator& operator=(const range_enumerator& other) = default;
	range_enumerator& operator=(range_enumerator&& other) = default;

	range_enumerator step(std::size_t step);

	bool operator++();
	const TValue& operator*() const;
	const TValue* operator->() const;

private:
	bool move_next() override
	{
		return this->operator++();
	}
	const TValue& current() const override
	{
		return this->operator*();
	}

	TValue _current;
	TValue _end;
	std::size_t _step;
	bool _firstMoveNext;
};

// ==============================================================================================

template <typename TValue>
range_enumerator<TValue> range(TValue begin, TValue end)
{
	return range_enumerator<TValue>(begin, end);
}

template <typename TValue>
range_enumerator<TValue>::range_enumerator()
	: TBaseClass(true)
{ }

template <typename TValue>
range_enumerator<TValue>::range_enumerator(TValue begin, TValue end)
	: TBaseClass(false)
	, _current(begin)
	, _end(end)
	, _step(1)
	, _firstMoveNext(true)
{
	auto containerEndHasBeenReached = _current == _end;
	TBaseClass::exhausted(containerEndHasBeenReached);
}

template <typename TValue>
range_enumerator<TValue> range_enumerator<TValue>::step(std::size_t step)
{
	_step = step;
	return *this;
}

template <typename TValue>
bool range_enumerator<TValue>::operator++()
{
	if(TBaseClass::exhausted())
		return TBaseClass::is_valid();

	if(_firstMoveNext)
		_firstMoveNext = false;
	else
		for(auto stepsLeft = _step; stepsLeft > 0 && _current != _end; --stepsLeft)
			++_current;

	auto containerEndHasBeenReached = _current == _end;
	TBaseClass::exhausted(containerEndHasBeenReached);
	return TBaseClass::is_valid();
}

template <typename TValue>
const TValue& range_enumerator<TValue>::operator*() const
{
	return _current;
}

template <typename TValue>
const TValue* range_enumerator<TValue>::operator->() const
{
	return &operator*();
}
} // namespace DefQuery
