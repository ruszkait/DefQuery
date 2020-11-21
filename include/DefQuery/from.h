#pragma once

#include <DefQuery/enumerator.h>

namespace DefQuery
{
template <typename TIterator, typename TValue>
class from_enumerator : public enumerator<TValue, from_enumerator<TIterator, TValue>>
{
public:
	using TBaseClass = enumerator<TValue, from_enumerator<TIterator, TValue>>;

	from_enumerator();
	from_enumerator(TIterator begin, TIterator end);

	from_enumerator(const from_enumerator& other) = default;
	from_enumerator(from_enumerator&& other) = default;
	from_enumerator& operator=(const from_enumerator& other) = default;
	from_enumerator& operator=(from_enumerator&& other) = default;

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

	TIterator _current;
	TIterator _end;
	bool _firstMoveNext;
};

// ==============================================================================================

template <typename TContainer>
from_enumerator<typename TContainer::const_iterator,
				typename TContainer::const_iterator::value_type>
from(const TContainer& container)
{
	return from_enumerator<typename TContainer::const_iterator,
						   typename TContainer::const_iterator::value_type>(container.begin(),
																			container.end());
}

template <typename TIterator, typename TValue>
from_enumerator<TIterator, TValue>::from_enumerator()
	: TBaseClass(true)
{ }

template <typename TIterator, typename TValue>
from_enumerator<TIterator, TValue>::from_enumerator(TIterator begin, TIterator end)
	: TBaseClass(false)
	, _current(begin)
	, _end(end)
	, _firstMoveNext(true)
{
	auto containerEndHasBeenReached = _current == _end;
	TBaseClass::exhausted(containerEndHasBeenReached);
}

template <typename TIterator, typename TValue>
bool from_enumerator<TIterator, TValue>::operator++()
{
	if(TBaseClass::exhausted())
		return TBaseClass::is_valid();

	if(_firstMoveNext)
		_firstMoveNext = false;
	else
		++_current;

	auto containerEndHasBeenReached = _current == _end;
	TBaseClass::exhausted(containerEndHasBeenReached);
	return TBaseClass::is_valid();
}

template <typename TIterator, typename TValue>
const TValue& from_enumerator<TIterator, TValue>::operator*() const
{
	return *_current;
}

template <typename TIterator, typename TValue>
const TValue* from_enumerator<TIterator, TValue>::operator->() const
{
	return &operator*();
}
} // namespace DefQuery
