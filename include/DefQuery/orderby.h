#pragma once

#include <DefQuery/enumerator.h>
#include <functional>
#include <queue>
#include <vector>

namespace DefQuery
{
enum class sorting_order
{
	ascending,
	descending
};

template <typename TSourceEnumerator>
class orderby_enumerator : public enumerator<typename TSourceEnumerator::value_type,
											 orderby_enumerator<TSourceEnumerator>>
{
public:
	using TComparator = std::function<int(const typename TSourceEnumerator::value_type&,
										  const typename TSourceEnumerator::value_type&)>;
	using TBaseClass =
		enumerator<typename TSourceEnumerator::value_type, orderby_enumerator<TSourceEnumerator>>;

	orderby_enumerator();

	template <typename TSourceEnumeratorConstr>
	orderby_enumerator(TSourceEnumeratorConstr&& source,
					   const TComparator& comparator,
					   sorting_order order);

	orderby_enumerator(const orderby_enumerator& other);
	orderby_enumerator(orderby_enumerator&& other);
	orderby_enumerator& operator=(const orderby_enumerator& other);
	orderby_enumerator& operator=(orderby_enumerator&& other);

	bool operator++();
	const typename TSourceEnumerator::value_type& operator*() const;
	const typename TSourceEnumerator::value_type* operator->() const;

	orderby_enumerator& thenby(sorting_order order, const TComparator& comparator);

private:
	bool move_next() override
	{
		return this->operator++();
	}
	const typename TSourceEnumerator::value_type& current() const override
	{
		return this->operator*();
	}

	bool greater(const typename TSourceEnumerator::value_type& left,
				 const typename TSourceEnumerator::value_type& right) const;

	TSourceEnumerator _source;
	struct SortingStage
	{
		TComparator _comparator;
		sorting_order _order;
	};
	std::vector<SortingStage> _sortingPolicies;
	std::priority_queue<typename TSourceEnumerator::value_type,
						std::vector<typename TSourceEnumerator::value_type>,
						std::function<bool(const typename TSourceEnumerator::value_type&,
										   const typename TSourceEnumerator::value_type&)>>
		_sortingWindow;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
orderby_enumerator<TDerived> enumerator<TValue, TDerived>::orderby(
	sorting_order order,
	const std::function<int(const value_type&, const value_type&)>& comparator) &&
{
	return orderby_enumerator<TDerived>(
		std::move(static_cast<TDerived&>(*this)), comparator, order);
}

template <typename TValue, typename TDerived>
orderby_enumerator<TDerived> enumerator<TValue, TDerived>::orderby(
	sorting_order order,
	const std::function<int(const value_type&, const value_type&)>& comparator) &
{
	return orderby_enumerator<TDerived>(static_cast<TDerived&>(*this), comparator, order);
}

template <class TStlAdapter>
const typename TStlAdapter::container_type& get_container(const TStlAdapter& adapter)
{
	struct AdapterProtectedMemberAccessor : TStlAdapter
	{
		static const typename TStlAdapter::container_type& get_container(const TStlAdapter& adapter)
		{
			return reinterpret_cast<const AdapterProtectedMemberAccessor&>(adapter).c;
		}
	};

	return AdapterProtectedMemberAccessor::get_container(adapter);
}

template <typename TSourceEnumerator>
orderby_enumerator<TSourceEnumerator>::orderby_enumerator()
	: TBaseClass(true)
{ }

template <typename TSourceEnumerator>
template <typename TSourceEnumeratorConstr>
orderby_enumerator<TSourceEnumerator>::orderby_enumerator(TSourceEnumeratorConstr&& source,
														  const TComparator& comparator,
														  sorting_order order)
	: TBaseClass(false)
	, _source(std::forward<TSourceEnumeratorConstr>(source))
	, _sortingPolicies({{comparator, order}})
	, _sortingWindow([this](const typename TSourceEnumerator::value_type& left,
							const typename TSourceEnumerator::value_type& right) {
		return this->greater(left, right);
	})
{ }

template <typename TSourceEnumerator>
orderby_enumerator<TSourceEnumerator>::orderby_enumerator(const orderby_enumerator& other)
	: TBaseClass(other)
	, _source(other._source)
	, _sortingPolicies(other._sortingPolicies)
	// The captured this pointer shall change when the priority queue gets copied/moved, so we have to do it manually
	// The contents of the sorting window is copied/moved further
	, _sortingWindow(
		  [this](const typename TSourceEnumerator::value_type& left,
				 const typename TSourceEnumerator::value_type& right) {
			  return this->greater(left, right);
		  },
		  get_container(other._sortingWindow))
{ }

template <typename TSourceEnumerator>
orderby_enumerator<TSourceEnumerator>::orderby_enumerator(orderby_enumerator&& other)
	: TBaseClass(std::move(other))
	, _source(std::move(other._source))
	, _sortingPolicies(std::move(other._sortingPolicies))
	// The captured this pointer shall change when the priority queue gets copied/moved, so we have to do it manually
	// The contents of the sorting window is copied/moved further
	, _sortingWindow(
		  [this](const typename TSourceEnumerator::value_type& left,
				 const typename TSourceEnumerator::value_type& right) {
			  return this->greater(left, right);
		  },
		  std::move(get_container(other._sortingWindow)))
{ }

template <typename TSourceEnumerator>
orderby_enumerator<TSourceEnumerator>&
orderby_enumerator<TSourceEnumerator>::operator=(const orderby_enumerator& other)
{
	if(&other == this)
		return this;

	TBaseClass::operator=(other);
	_source = other._source;
	_sortingPolicies = other._sortingPolicies;
	// The captured this pointer shall change when the priority queue gets copied/moved, so we have to do it manually
	// The contents of the sorting window is copied/moved further
	_sortingWindow = decltype(_sortingWindow)(
		[this](const typename TSourceEnumerator::value_type& left,
			   const typename TSourceEnumerator::value_type& right) {
			return this->greater(left, right);
		},
		get_container(other._sortingWindow));

	return this;
}

template <typename TSourceEnumerator>
orderby_enumerator<TSourceEnumerator>&
orderby_enumerator<TSourceEnumerator>::operator=(orderby_enumerator&& other)
{
	if(&other == this)
		return this;

	TBaseClass::operator=(std::move(other));
	_source = std::move(other._source);
	_sortingPolicies = std::move(other._sortingPolicies);
	// The captured this pointer shall change when the priority queue gets copied/moved, so we have to do it manually
	// The contents of the sorting window is copied/moved further
	_sortingWindow = decltype(_sortingWindow)(
		[this](const typename TSourceEnumerator::value_type& left,
			   const typename TSourceEnumerator::value_type& right) {
			return this->greater(left, right);
		},
		std::move(get_container(other._sortingWindow)));

	return this;
}

template <typename TSourceEnumerator>
bool orderby_enumerator<TSourceEnumerator>::operator++()
{
	if(TBaseClass::exhausted())
		return TBaseClass::is_valid();

	auto topElementWasAlreadyConsumed = !_sortingWindow.empty();
	if(topElementWasAlreadyConsumed)
		_sortingWindow.pop();
	else
		while(++_source)
			_sortingWindow.emplace(*_source);

	TBaseClass::exhausted(_sortingWindow.empty());
	return TBaseClass::is_valid();
}

template <typename TSourceEnumerator>
const typename TSourceEnumerator::value_type&
orderby_enumerator<TSourceEnumerator>::operator*() const
{
	return _sortingWindow.top();
}

template <typename TSourceEnumerator>
const typename TSourceEnumerator::value_type*
orderby_enumerator<TSourceEnumerator>::operator->() const
{
	return &operator*();
}

template <typename TSourceEnumerator>
orderby_enumerator<TSourceEnumerator>&
orderby_enumerator<TSourceEnumerator>::thenby(sorting_order order, const TComparator& comparator)
{
	_sortingPolicies.emplace_back(SortingStage{comparator, order});
	return *this;
}

template <typename TSourceEnumerator>
bool orderby_enumerator<TSourceEnumerator>::greater(
	const typename TSourceEnumerator::value_type& left,
	const typename TSourceEnumerator::value_type& right) const
{
	for(auto& sortingPolicy : _sortingPolicies)
	{
		auto comparisonResult = sortingPolicy._comparator(left, right);

		if(sortingPolicy._order == sorting_order::descending)
			comparisonResult = -comparisonResult;

		auto leftAndRightAreTheSame = comparisonResult == 0;
		if(leftAndRightAreTheSame)
			continue;

		auto leftIsGreaterThanRight = comparisonResult > 0;
		return leftIsGreaterThanRight;
	}

	return false;
}
} // namespace DefQuery
