#pragma once

#include "enumerator.h"

namespace DefQuery
{
template <typename TSourceEnumerator>
class take_enumerator
	: public enumerator<typename TSourceEnumerator::value_type, take_enumerator<TSourceEnumerator>>
{
public:
	using TBaseClass =
		enumerator<typename TSourceEnumerator::value_type, take_enumerator<TSourceEnumerator>>;

	take_enumerator();

	template <typename TSourceEnumeratorConstr>
	take_enumerator(TSourceEnumeratorConstr&& source, std::size_t itemsToTake);

	take_enumerator(const take_enumerator& other) = default;
	take_enumerator(take_enumerator&& other) = default;
	take_enumerator& operator=(const take_enumerator& other) = default;
	take_enumerator& operator=(take_enumerator&& other) = default;

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
	std::size_t _itemsToTake;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
take_enumerator<TDerived> enumerator<TValue, TDerived>::take(std::size_t itemsToTake) &&
{
	return take_enumerator<TDerived>(std::move(static_cast<TDerived&>(*this)), itemsToTake);
}

template <typename TValue, typename TDerived>
take_enumerator<TDerived> enumerator<TValue, TDerived>::take(std::size_t itemsToTake) &
{
	return take_enumerator<TDerived>(static_cast<TDerived&>(*this), itemsToTake);
}

template <typename TSourceEnumerator>
take_enumerator<TSourceEnumerator>::take_enumerator()
	: TBaseClass(true)
{ }

template <typename TSourceEnumerator>
template <typename TSourceEnumeratorConstr>
take_enumerator<TSourceEnumerator>::take_enumerator(TSourceEnumeratorConstr&& source,
													std::size_t itemsToTake)
	: TBaseClass(false)
	, _source(std::forward<TSourceEnumeratorConstr>(source))
	, _itemsToTake(itemsToTake)
{ }

template <typename TSourceEnumerator>
bool take_enumerator<TSourceEnumerator>::operator++()
{
	if(TBaseClass::exhausted())
		return TBaseClass::is_valid();

	auto sourceExhausted = !(++_source);
	auto noMoreToTake = _itemsToTake-- == 0;
	if(sourceExhausted || noMoreToTake)
		TBaseClass::exhausted(true);

	return TBaseClass::is_valid();
}

template <typename TSourceEnumerator>
const typename TSourceEnumerator::value_type& take_enumerator<TSourceEnumerator>::operator*() const
{
	return *_source;
}

template <typename TSourceEnumerator>
const typename TSourceEnumerator::value_type* take_enumerator<TSourceEnumerator>::operator->() const
{
	return &operator*();
}
} // namespace DefQuery
