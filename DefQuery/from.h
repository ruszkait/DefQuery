#include "enumerator.h"

template <typename TIterator, typename TValue>
class from_enumerator : public enumerator<TValue, from_enumerator<TIterator, TValue>>
{
public:
	from_enumerator(TIterator begin, TIterator end);

	bool moveNext() override;
	const value_type& current() const override;

protected:
	enumerator_interface<value_type>* clone() const override;

private:
	typename TIterator _current;
	typename TIterator _end;
	bool _firstMoveNext;
};

// ==============================================================================================

template <typename TValue>
from_enumerator<TValue*, TValue> from(TValue* begin, TValue* end)
{
	return from_enumerator<TValue*, TValue>(begin, end);
}

template <typename TIterator>
from_enumerator<TIterator, typename TIterator::value_type> from(TIterator begin, TIterator end)
{
	return from_enumerator<TIterator, typename TIterator::value_type>(begin, end);
}

template <typename TContainer>
from_enumerator<typename TContainer::iterator, typename TContainer::iterator::value_type> from(TContainer& container)
{
	return from(container.begin(), container.end());
}

template<typename TIterator, typename TValueType>
from_enumerator<TIterator, TValueType>::from_enumerator(TIterator begin, TIterator end)
	: _current(begin)
	, _end(end)
	, _firstMoveNext(true)
{
}

template<typename TIterator, typename TValueType>
bool from_enumerator<TIterator, TValueType>::moveNext()
{
	auto containerEndHasBeenReached = _current == _end;
	if (containerEndHasBeenReached)
		return false;

	if (_firstMoveNext)
		_firstMoveNext = false;
	else
		++_current;

	auto containerEndHasNotBeenReached = _current != _end;
	return containerEndHasNotBeenReached;
}

template<typename TIterator, typename TValue>
const typename from_enumerator<TIterator, TValue>::value_type& from_enumerator<TIterator, TValue>::current() const
{
	return *_current;
}

template<typename TIterator, typename TValueType>
enumerator_interface<typename from_enumerator<TIterator, TValueType>::value_type>* from_enumerator<TIterator, TValueType>::clone() const
{
	return new from_enumerator<TIterator, TValueType>(*this);
}