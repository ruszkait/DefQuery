#pragma once

#include <memory>

template <typename TSourceEnumerator, typename TFilter>
class where_enumerator;

template <typename TValue>
class shared_enumerator;


template <typename TValue>
class enumerator_interface
{
public:
	using value_type = TValue;

	virtual ~enumerator_interface() = default;

	virtual bool moveNext() = 0;
	virtual value_type& current() const = 0;

protected:
	virtual enumerator_interface<value_type>* clone() const = 0;
};

template <typename TValue, typename TDerived>
class enumerator : public enumerator_interface<TValue>
{
public:
	using derived_type = TDerived;

	template <typename TFilter>
	where_enumerator<derived_type, TFilter> where(const TFilter& filter);

	std::shared_ptr<shared_enumerator<TValue>> share();
};
