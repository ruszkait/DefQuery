#pragma once

#include <memory>

template <typename TSourceEnumerator, typename TFilter>
class where_enumerator;

template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
class select_enumerator;

template <typename TSourceEnumerator>
class stlrange_adapter;

template <typename TValue>
class shared_enumerator;


template <typename TValue>
class enumerator_interface
{
public:
	using value_type = TValue;

	virtual ~enumerator_interface() = default;

	// Produces the next element, returns false if the enumeration has exhausted
	virtual bool operator++() = 0;

	// Provides the last produced element
	virtual const value_type& operator*() const = 0;

protected:
	virtual enumerator_interface<value_type>* clone() const = 0;
};

template <typename TValue, typename TDerived>
class enumerator : public enumerator_interface<TValue>
{
public:
	using derived_type = TDerived;

	// Keeps the data that passes the filter predicate in the stream
	template <typename TFilter>
	where_enumerator<derived_type, TFilter> where(const TFilter& filter);

	// Transforms the source value to another type
	template <typename TProjection, typename TProjectedValue = std::result_of<TProjection(value_type&)>::type>
//	template <typename TProjection, typename TProjectedValue = std::invoke_result<TProjection, value_type&>::type>
	select_enumerator<derived_type, TProjection, TProjectedValue> select(const TProjection& projector);

	// Flattens out a hierarchical container structure
	//template <typename TEnumeratorProjection, typename TProjectedEnumeratorValue = std::result_of<TEnumeratorProjection(value_type&)>::type::value_type>
	//selectmany_enumerator<derived_type, TEnumeratorProjection, TProjectedEnumeratorValue> selectmany(const TEnumeratorProjection& projector);

	// Erases the underlying decorator chain type and provides an stream value oriented interface.
	// Creates a shared pointer wrapper. This wrapper can be passed around by value cheap.
	// Then the shared enumerator can be further decorated by other enumerators.
	shared_enumerator<value_type> share();

	// Creates an STL range from the enumerator
	stlrange_adapter<derived_type> stlrange();
};
