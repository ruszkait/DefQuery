#pragma once

#include "enumerator.h"
#include <functional>
#include <queue>
#include <stack>
#include <utility>

namespace DefQuery
{
template <typename TSourceEnumerator>
class reverse_enumerator : public enumerator<typename TSourceEnumerator::value_type,
											 reverse_enumerator<TSourceEnumerator>>
{
public:
	using TBaseClass =
		enumerator<typename TSourceEnumerator::value_type, reverse_enumerator<TSourceEnumerator>>;

	reverse_enumerator();

	reverse_enumerator(const TSourceEnumerator& source);
	reverse_enumerator(TSourceEnumerator&& source);

	reverse_enumerator(const reverse_enumerator& other) = default;
	reverse_enumerator(reverse_enumerator&& other) = default;
	reverse_enumerator& operator=(const reverse_enumerator& other) = default;
	reverse_enumerator& operator=(reverse_enumerator&& other) = default;

	bool operator++();
	const typename TSourceEnumerator::value_type& operator*() const;
	const typename TSourceEnumerator::value_type* operator->() const;

private:
	bool move_next() override
	{
		return this->operator++();
	}
	const typename TSourceEnumerator::value_type& current() const override
	{
		return this->operator*();
	}

	TSourceEnumerator _source;
	std::stack<typename TSourceEnumerator::value_type> _reversingWindow;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
reverse_enumerator<TDerived> enumerator<TValue, TDerived>::reverse() &&
{
	return reverse_enumerator<TDerived>(std::move(static_cast<TDerived&>(*this)));
}

template <typename TValue, typename TDerived>
reverse_enumerator<TDerived> enumerator<TValue, TDerived>::reverse() &
{
	return reverse_enumerator<TDerived>(static_cast<TDerived&>(*this));
}

template <typename TSourceEnumerator>
reverse_enumerator<TSourceEnumerator>::reverse_enumerator()
	: TBaseClass(true)
{ }

template <typename TSourceEnumerator>
reverse_enumerator<TSourceEnumerator>::reverse_enumerator(const TSourceEnumerator& source)
	: TBaseClass(false)
	, _source(source)
{ }

template <typename TSourceEnumerator>
reverse_enumerator<TSourceEnumerator>::reverse_enumerator(TSourceEnumerator&& source)
	: TBaseClass(false)
	, _source(std::move(source))
{ }

template <typename TSourceEnumerator>
bool reverse_enumerator<TSourceEnumerator>::operator++()
{
	if(TBaseClass::exhausted())
		return TBaseClass::is_valid();

	auto topElementWasAlreadyConsumed = !_reversingWindow.empty();
	if(topElementWasAlreadyConsumed)
		_reversingWindow.pop();
	else
		while(++_source)
			_reversingWindow.emplace(*_source);

	TBaseClass::exhausted(_reversingWindow.empty());
	return TBaseClass::is_valid();
}

template <typename TSourceEnumerator>
const typename TSourceEnumerator::value_type&
reverse_enumerator<TSourceEnumerator>::operator*() const
{
	return _reversingWindow.top();
}

template <typename TSourceEnumerator>
const typename TSourceEnumerator::value_type*
reverse_enumerator<TSourceEnumerator>::operator->() const
{
	return &operator*();
}
} // namespace DefQuery
