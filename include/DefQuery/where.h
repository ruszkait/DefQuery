#pragma once

#include "enumerator.h"

namespace DefQuery
{
template <typename TSourceEnumerator, typename TFilter>
class where_enumerator : public enumerator<typename TSourceEnumerator::value_type,
										   where_enumerator<TSourceEnumerator, TFilter>>
{
public:
	using TBaseClass = enumerator<typename TSourceEnumerator::value_type,
								  where_enumerator<TSourceEnumerator, TFilter>>;

	where_enumerator();

	template <typename TSourceEnumeratorConstr, typename TFilterConstr>
	where_enumerator(TSourceEnumeratorConstr&& source, TFilterConstr&& filter);

	where_enumerator(const where_enumerator& other) = default;
	where_enumerator(where_enumerator&& other) = default;
	where_enumerator& operator=(const where_enumerator& other) = default;
	where_enumerator& operator=(where_enumerator&& other) = default;

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
	TFilter _filter;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
template <typename TFilter>
where_enumerator<TDerived, TFilter> enumerator<TValue, TDerived>::where(TFilter&& filter) &&
{
	return where_enumerator<TDerived, TFilter>(std::move(static_cast<TDerived&>(*this)),
											   std::forward<TFilter>(filter));
}

template <typename TValue, typename TDerived>
template <typename TFilter>
where_enumerator<TDerived, TFilter> enumerator<TValue, TDerived>::where(TFilter&& filter) &
{
	return where_enumerator<TDerived, TFilter>(static_cast<TDerived&>(*this),
											   std::forward<TFilter>(filter));
}

template <typename TSourceEnumerator, typename TFilter>
where_enumerator<TSourceEnumerator, TFilter>::where_enumerator()
	: TBaseClass(true)
{ }

template <typename TSourceEnumerator, typename TFilter>
template <typename TSourceEnumeratorConstr, typename TFilterConstr>
where_enumerator<TSourceEnumerator, TFilter>::where_enumerator(TSourceEnumeratorConstr&& source,
															   TFilterConstr&& filter)
	: TBaseClass(false)
	, _source(std::forward<TSourceEnumeratorConstr>(source))
	, _filter(std::forward<TFilterConstr>(filter))
{ }

template <typename TSourceEnumerator, typename TFilter>
bool where_enumerator<TSourceEnumerator, TFilter>::operator++()
{
	if(TBaseClass::exhausted())
		return TBaseClass::is_valid();

	while(++_source)
	{
		auto itemPassedTheFilter = _filter(*_source);
		if(itemPassedTheFilter)
			return TBaseClass::is_valid();
	}

	TBaseClass::exhausted(true);
	return TBaseClass::is_valid();
}

template <typename TSourceEnumerator, typename TFilter>
const typename TSourceEnumerator::value_type&
where_enumerator<TSourceEnumerator, TFilter>::operator*() const
{
	return *_source;
}

template <typename TSourceEnumerator, typename TFilter>
const typename TSourceEnumerator::value_type*
where_enumerator<TSourceEnumerator, TFilter>::operator->() const
{
	return &operator*();
}
} // namespace DefQuery
