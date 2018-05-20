template <typename TSourceEnumerator, typename TFilter>
class where_enumerator;

#pragma once

template <typename T, typename TDerived>
class enumerator
{
public:
	using Type = T;
	using DerivedType = TDerived;

	virtual bool moveNext() = 0;
	virtual Type& current() const = 0;

	template <typename TFilter>
	where_enumerator<DerivedType, TFilter> where(TFilter filter);
};
