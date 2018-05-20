#pragma once

#include <memory>

template <typename TSourceEnumerator, typename TFilter>
class where_enumerator;

template <typename T>
class shared_enumerator;


template <typename T>
class enumerator_interface
{
public:
	using Type = T;

	virtual ~enumerator_interface() = default;

	virtual bool moveNext() = 0;
	virtual Type& current() const = 0;

protected:
	virtual enumerator_interface<Type>* clone() const = 0;
};

template <typename T, typename TDerived>
class enumerator : public enumerator_interface<T>
{
public:
	using DerivedType = TDerived;

	template <typename TFilter>
	where_enumerator<DerivedType, TFilter> where(const TFilter& filter);

	std::shared_ptr<shared_enumerator<T>> share();
};
