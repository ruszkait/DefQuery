#pragma once

#include "enumerator.h"
#include <deque>

namespace DefQuery
{
template <typename TKey, typename TMember>
struct group
{
	group(const TKey& key, const TMember& member)
		: _key(key)
		, _members({member})
	{ }

	TKey _key;
	std::vector<TMember> _members;
};

template <typename TSourceEnumerator, typename TKeyProjection>
class groupby_enumerator
	: public enumerator<group<typename std::result_of<TKeyProjection(
								  const typename TSourceEnumerator::value_type&)>::type,
							  typename TSourceEnumerator::value_type>,
						groupby_enumerator<TSourceEnumerator, TKeyProjection>>
{
public:
	using TProjectedKey = typename std::result_of<TKeyProjection(
		const typename TSourceEnumerator::value_type&)>::type;
	using TBaseClass = enumerator<group<typename std::result_of<TKeyProjection(
											const typename TSourceEnumerator::value_type&)>::type,
										typename TSourceEnumerator::value_type>,
								  groupby_enumerator<TSourceEnumerator, TKeyProjection>>;

	groupby_enumerator();

	template <typename TSourceEnumeratorConstr, typename TProjectionConstr>
	groupby_enumerator(TSourceEnumeratorConstr&& source, TProjectionConstr&& keyProjection);

	groupby_enumerator(const groupby_enumerator& other);
	groupby_enumerator(groupby_enumerator&& other) = default;
	groupby_enumerator& operator=(const groupby_enumerator& other);
	groupby_enumerator& operator=(groupby_enumerator&& other) = default;

	bool operator++();
	const group<TProjectedKey, typename TSourceEnumerator::value_type>& operator*() const;
	const group<TProjectedKey, typename TSourceEnumerator::value_type>* operator->() const;

private:
	bool move_next() override
	{
		return this->operator++();
	}
	const group<TProjectedKey, typename TSourceEnumerator::value_type>& current() const override
	{
		return this->operator*();
	}

	TSourceEnumerator _source;
	TKeyProjection _keyProjector;
	std::deque<group<TProjectedKey, typename TSourceEnumerator::value_type>> _groups;
	typename decltype(_groups)::const_iterator _currentGroup;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
template <typename TKeyProjection>
groupby_enumerator<TDerived, TKeyProjection>
enumerator<TValue, TDerived>::groupby(TKeyProjection&& keyProjector) &&
{
	return groupby_enumerator<TDerived, TKeyProjection>(std::move(static_cast<TDerived&>(*this)),
														std::forward<TKeyProjection>(keyProjector));
}

template <typename TValue, typename TDerived>
template <typename TKeyProjection>
groupby_enumerator<TDerived, TKeyProjection>
enumerator<TValue, TDerived>::groupby(TKeyProjection&& keyProjector) &
{
	return groupby_enumerator<TDerived, TKeyProjection>(std::move(static_cast<TDerived&>(*this)),
														std::forward<TKeyProjection>(keyProjector));
}

template <typename TSourceEnumerator, typename TKeyProjection>
groupby_enumerator<TSourceEnumerator, TKeyProjection>::groupby_enumerator()
	: TBaseClass(true)
{ }

template <typename TSourceEnumerator, typename TKeyProjection>
template <typename TSourceEnumeratorConstr, typename TKeyProjectionConstr>
groupby_enumerator<TSourceEnumerator, TKeyProjection>::groupby_enumerator(
	TSourceEnumeratorConstr&& source, TKeyProjectionConstr&& keyProjection)
	: TBaseClass(false)
	, _source(std::forward<TSourceEnumeratorConstr>(source))
	, _keyProjector(std::forward<TKeyProjectionConstr>(keyProjection))
	, _currentGroup(_groups.cend())
{ }

template <typename TSourceEnumerator, typename TKeyProjection>
groupby_enumerator<TSourceEnumerator, TKeyProjection>::groupby_enumerator(
	const groupby_enumerator& other)
	: TBaseClass(other)
	, _source(other._source)
	, _keyProjector(other._keyProjector)
	, _groups(other._groups)
	, _currentGroup(_groups.cbegin() + (other._currentGroup - other._groups.cbegin()))
{ }

template <typename TSourceEnumerator, typename TKeyProjection>
groupby_enumerator<TSourceEnumerator, TKeyProjection>&
groupby_enumerator<TSourceEnumerator, TKeyProjection>::operator=(const groupby_enumerator& other)
{
	if(&other == this)
		return this;

	TBaseClass::operator==(other);
	_source = other._source;
	_keyProjector = other._keyProjector;
	_groups = other._groups;
	_currentGroup = _groups.cbegin() + (other._currentGroup - other._groups.cbegin());

	return this;
}

template <typename TSourceEnumerator, typename TKeyProjection>
bool groupby_enumerator<TSourceEnumerator, TKeyProjection>::operator++()
{
	if(TBaseClass::exhausted())
		return TBaseClass::is_valid();

	auto groupsMustBeInitialized = _currentGroup == _groups.cend();
	if(groupsMustBeInitialized)
	{
		while(++_source)
		{
			const auto& elementKey = _keyProjector(*_source);
			auto foundIter =
				std::find_if(_groups.begin(), _groups.end(), [&elementKey](const auto& group) {
					return group._key == elementKey;
				});
			auto newGroup = foundIter == _groups.end();
			if(newGroup)
				_groups.emplace_back(elementKey, *_source);
			else
				foundIter->_members.emplace_back(*_source);
		}
		_currentGroup = _groups.cbegin();
	}
	else
		++_currentGroup;

	TBaseClass::exhausted(_currentGroup == _groups.cend());
	return TBaseClass::is_valid();
}

template <typename TSourceEnumerator, typename TKeyProjection>
const group<
	typename std::result_of<TKeyProjection(const typename TSourceEnumerator::value_type&)>::type,
	typename TSourceEnumerator::value_type>&
groupby_enumerator<TSourceEnumerator, TKeyProjection>::operator*() const
{
	return *_currentGroup;
}

template <typename TSourceEnumerator, typename TKeyProjection>
const group<
	typename std::result_of<TKeyProjection(const typename TSourceEnumerator::value_type&)>::type,
	typename TSourceEnumerator::value_type>*
groupby_enumerator<TSourceEnumerator, TKeyProjection>::operator->() const
{
	return &operator*();
}
} // namespace DefQuery
