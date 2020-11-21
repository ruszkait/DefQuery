#pragma once

#include <functional>
#include <list>
#include <memory>
#include <type_traits>
#include <vector>

namespace DefQuery
{
template <typename TSourceEnumerator, typename TFilter>
class where_enumerator;

template <typename TSourceEnumerator, typename TProjection>
class select_enumerator;

template <typename TSourceEnumerator, typename TEnumeratorProjection>
class selectmany_enumerator;

template <typename TSourceEnumerator>
class stlrange_adapter;

template <typename TValue>
class decayed_enumerator;

template <typename TSourceEnumerator>
class orderby_enumerator;
enum class sorting_order;

template <typename TLeftEnumerator,
		  typename TRightEnumerator,
		  typename TKeyProjectionLeft,
		  typename TKeyProjectionRight,
		  typename TMerging>
class join_enumerator;

template <typename TSourceEnumerator>
class reverse_enumerator;

template <typename TSourceEnumerator>
class take_enumerator;

template <typename TSourceEnumerator, typename TStopCondition>
class takewhile_enumerator;

template <typename TSourceEnumerator, typename TKeyProjection>
class groupby_enumerator;

template <typename TValue>
class enumerator_interface
{
public:
	virtual ~enumerator_interface() = default;

	// Produces the next element, returns false if the enumeration has exhausted
	// Prefer the usage of the enumerator operator++() over this function,
	// this saves the cost of calling a virtual function
	virtual bool move_next() = 0;

	// Provides the last produced element
	// Prefer the usage of the enumerator operator*() over this function,
	// this saves the cost of calling a virtual function
	virtual const TValue& current() const = 0;

	// Creates a clone of the enumerator on the heap
	virtual enumerator_interface* clone() const = 0;
};

template <typename TValue, typename TDerived>
class enumerator : public enumerator_interface<TValue>
{
public:
	using value_type = TValue;

	// Keeps the data that passes the filter predicate in the stream
	template <typename TFilter>
	where_enumerator<TDerived, TFilter> where(TFilter&& filter) &;
	template <typename TFilter>
	where_enumerator<TDerived, TFilter> where(TFilter&& filter) &&;

	// Transforms the source value to another type
	template <typename TProjection>
	select_enumerator<TDerived, TProjection> select(TProjection&& projector) &;
	template <typename TProjection>
	select_enumerator<TDerived, TProjection> select(TProjection&& projector) &&;

	// Flattens out a hierarchical container structure
	template <typename TEnumeratorProjection>
	selectmany_enumerator<TDerived, TEnumeratorProjection>
	selectmany(const TEnumeratorProjection& projector) &;
	template <typename TEnumeratorProjection>
	selectmany_enumerator<TDerived, TEnumeratorProjection>
	selectmany(const TEnumeratorProjection& projector) &&;

	// Erases the underlying decorator chain type. This also means that the undelying chain goes to the heap
	decayed_enumerator<TValue> decay() &;
	decayed_enumerator<TValue> decay() &&;

	// Creates an STL range from the enumerator
	stlrange_adapter<TDerived> stlrange() &;
	stlrange_adapter<TDerived> stlrange() &&;

	// Accumulates the values of the underlying enumerator to an accumlator
	template <typename TAccumlatorInitializer, typename TFolding>
	typename std::result_of<TAccumlatorInitializer(const TValue&)>::type
	aggregate(const TFolding& folder, const TAccumlatorInitializer& accumlatorInitializer);

	// Returns the number of elements
	std::size_t count();

	// Returns if any element fulfills the search criteria
	template <typename TFilter>
	bool contains(const TFilter& filter);
	bool contains(const TValue& searched);

	// Creates a container out of the query results
	template <typename TContainer>
	TContainer store(const std::function<void(TContainer&, const TValue&)>& containerInserter);

	// Creates a list out of the query results
	std::list<TValue> list();

	// Creates a vector out of the query results
	std::vector<TValue> vector();

	// Sorting with a certain window size
	orderby_enumerator<TDerived> orderby(
		sorting_order order,
		const std::function<int(const TValue&, const TValue&)>& comparator = [](const auto& left,
																				const auto& right) {
			return left == right ? 0 : (left < right ? -1 : +1);
		}) &;
	orderby_enumerator<TDerived> orderby(
		sorting_order order,
		const std::function<int(const TValue&, const TValue&)>& comparator = [](const auto& left,
																				const auto& right) {
			return left == right ? 0 : (left < right ? -1 : +1);
		}) &&;

	// Returns the first element of the enumeration
	TValue first();
	TValue first_or_default();

	// Joins two enumerators based on their keys
	template <typename TRightEnumerator,
			  typename TKeyProjectionLeft,
			  typename TKeyProjectionRight,
			  typename TMerging>
	join_enumerator<TDerived, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>
	join(TRightEnumerator&& sourceRight,
		 TKeyProjectionLeft&& keyProjectionLeft,
		 TKeyProjectionRight&& keyProjectionRight,
		 TMerging&& merging) &;
	template <typename TRightEnumerator,
			  typename TKeyProjectionLeft,
			  typename TKeyProjectionRight,
			  typename TMerging>
	join_enumerator<TDerived, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>
	join(TRightEnumerator&& sourceRight,
		 TKeyProjectionLeft&& keyProjectionLeft,
		 TKeyProjectionRight&& keyProjectionRight,
		 TMerging&& merging) &&;

	// Returns if all element of the sequence fullfills a requirement
	template <typename TFilter>
	bool all(const TFilter& filter);

	// Returns if any element of the sequence fullfills a requirement
	template <typename TFilter>
	bool any(const TFilter& filter);

	// Reverses the order of the elements
	reverse_enumerator<TDerived> reverse() &;
	reverse_enumerator<TDerived> reverse() &&;

	// Takes the first N element from the sequence
	take_enumerator<TDerived> take(std::size_t itemsToTake) &;
	take_enumerator<TDerived> take(std::size_t itemsToTake) &&;

	// Takes elements as long as the condition is true
	template <typename TRunningConditionConstr>
	takewhile_enumerator<TDerived, TRunningConditionConstr>
	takewhile(TRunningConditionConstr&& runningCondition) &;
	template <typename TRunningConditionConstr>
	takewhile_enumerator<TDerived, TRunningConditionConstr>
	takewhile(TRunningConditionConstr&& runningCondition) &&;

	// Grouping of elements based on a key
	template <typename TKeyProjection>
	groupby_enumerator<TDerived, TKeyProjection> groupby(TKeyProjection&& keyProjector) &;
	template <typename TKeyProjection>
	groupby_enumerator<TDerived, TKeyProjection> groupby(TKeyProjection&& keyProjector) &&;

protected:
	enumerator(bool exhausted);

	enumerator(const enumerator& other) = default;
	enumerator(enumerator&& other);
	enumerator& operator=(const enumerator& other) = default;
	enumerator& operator=(enumerator&& other);

	enumerator_interface<TValue>* clone() const override;

	bool exhausted() const
	{
		return _exhausted;
	}
	bool is_valid() const
	{
		return !_exhausted;
	}
	void exhausted(bool value)
	{
		_exhausted = value;
	}

	bool _exhausted;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
enumerator<TValue, TDerived>::enumerator(bool exhausted)
	: _exhausted(exhausted)
{ }

template <typename TValue, typename TDerived>
enumerator<TValue, TDerived>::enumerator(enumerator&& other)
	: _exhausted(other._exhausted)
{
	other._exhausted = true;
}

template <typename TValue, typename TDerived>
enumerator<TValue, TDerived>& enumerator<TValue, TDerived>::operator=(enumerator&& other)
{
	if(this == &other)
		return *this;

	_exhausted = other._exhausted;
	other._exhausted = true;

	return *this;
}

template <typename TValue, typename TDerived>
enumerator_interface<TValue>* enumerator<TValue, TDerived>::clone() const
{
	return new TDerived(static_cast<const TDerived&>(*this));
}
} // namespace DefQuery
