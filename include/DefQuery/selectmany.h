#pragma once

#include <DefQuery/enumerator.h>

namespace DefQuery
{
template <typename TSourceEnumerator, typename TEnumeratorProjection>
class selectmany_enumerator
	: public enumerator<typename std::result_of<TEnumeratorProjection(
							const typename TSourceEnumerator::value_type&)>::type::value_type,
						selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection>>
{
public:
	using TProjectedEnumerator = typename std::result_of<TEnumeratorProjection(
		const typename TSourceEnumerator::value_type&)>::type;
	using TBaseClass =
		enumerator<typename std::result_of<TEnumeratorProjection(
					   const typename TSourceEnumerator::value_type&)>::type::value_type,
				   selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection>>;

	selectmany_enumerator();

	template <typename TSourceEnumeratorConstr>
	selectmany_enumerator(TSourceEnumeratorConstr&& source,
						  const TEnumeratorProjection& projection);

	selectmany_enumerator(const selectmany_enumerator& other) = default;
	selectmany_enumerator(selectmany_enumerator&& other) = default;
	selectmany_enumerator& operator=(const selectmany_enumerator& other) = default;
	selectmany_enumerator& operator=(selectmany_enumerator&& other) = default;

	bool operator++();
	const typename TProjectedEnumerator::value_type& operator*() const;
	const typename TProjectedEnumerator::value_type* operator->() const;

private:
	bool move_next() override
	{
		return this->operator++();
	}
	const typename TProjectedEnumerator::value_type& current() const override
	{
		return this->operator*();
	}

	TSourceEnumerator _source;
	TEnumeratorProjection _projector;
	TProjectedEnumerator _currentProjectedEnumerator;
	typename TProjectedEnumerator::value_type _currentProjectedValue;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
template <typename TEnumeratorProjection>
selectmany_enumerator<TDerived, TEnumeratorProjection>
enumerator<TValue, TDerived>::selectmany(const TEnumeratorProjection& projector) &&
{
	return selectmany_enumerator<TDerived, TEnumeratorProjection>(
		std::move(static_cast<TDerived&>(*this)), projector);
}

template <typename TValue, typename TDerived>
template <typename TEnumeratorProjection>
selectmany_enumerator<TDerived, TEnumeratorProjection>
enumerator<TValue, TDerived>::selectmany(const TEnumeratorProjection& projector) &
{
	return selectmany_enumerator<TDerived, TEnumeratorProjection>(static_cast<TDerived&>(*this),
																  projector);
}

template <typename TSourceEnumerator, typename TEnumeratorProjection>
selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection>::selectmany_enumerator()
	: TBaseClass(true)
{ }

template <typename TSourceEnumerator, typename TEnumeratorProjection>
template <typename TSourceEnumeratorConstr>
selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection>::selectmany_enumerator(
	TSourceEnumeratorConstr&& source, const TEnumeratorProjection& projector)
	: TBaseClass(false)
	, _source(std::forward<TSourceEnumeratorConstr>(source))
	, _projector(projector)
{ }

template <typename TSourceEnumerator, typename TEnumeratorProjection>
bool selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection>::operator++()
{
	if(TBaseClass::exhausted())
		return TBaseClass::is_valid();

	auto currentProjectedEnumeratorExhausted = !++_currentProjectedEnumerator;
	while(currentProjectedEnumeratorExhausted)
	{
		auto sourceIsExhausted = !++_source;
		TBaseClass::exhausted(sourceIsExhausted);
		if(sourceIsExhausted)
			return TBaseClass::is_valid();

		_currentProjectedEnumerator = _projector(*_source);
		currentProjectedEnumeratorExhausted = !++_currentProjectedEnumerator;
	}

	_currentProjectedValue = *_currentProjectedEnumerator;
	return TBaseClass::is_valid();
	;
}

template <typename TSourceEnumerator, typename TEnumeratorProjection>
const typename selectmany_enumerator<TSourceEnumerator,
									 TEnumeratorProjection>::TProjectedEnumerator::value_type&
selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection>::operator*() const
{
	return _currentProjectedValue;
}

template <typename TSourceEnumerator, typename TEnumeratorProjection>
const typename selectmany_enumerator<TSourceEnumerator,
									 TEnumeratorProjection>::TProjectedEnumerator::value_type*
selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection>::operator->() const
{
	return &operator*();
}
} // namespace DefQuery
