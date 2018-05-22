#include "enumerator.h"

template <typename TSourceEnumerator, typename TEnumeratorProjection, typename TProjectedEnumeratorValue>
class selectmany_enumerator : public enumerator<typename TProjectedEnumeratorValue, selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumeratorValue>>
{
public:
	using projected_enumerator_type = typename std::result_of<TEnumeratorProjection(const typename TSourceEnumerator::value_type&)>::type;

	selectmany_enumerator(const TSourceEnumerator& source, const TEnumeratorProjection& projection);

	bool operator++();
	const value_type& operator*() const;

private:
	bool move_next() override { return this->operator++(); }
	const value_type& current() const { return this->operator*(); }

	TSourceEnumerator _source;
	TEnumeratorProjection _projector;
	projected_enumerator_type _currentProjectedEnumerator;
	value_type _currentProjectedValue;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
template <typename TEnumeratorProjection, typename TProjectedEnumeratorValue>
selectmany_enumerator<TDerived, TEnumeratorProjection, TProjectedEnumeratorValue> enumerator<TValue, TDerived>::selectmany(const TEnumeratorProjection& projector)
{
	return selectmany_enumerator<TDerived, TEnumeratorProjection, TProjectedEnumeratorValue>(static_cast<TDerived&>(*this), projector);
}

template <typename TSourceEnumerator, typename TEnumeratorProjection, typename TProjectedEnumeratorValue>
selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumeratorValue>::selectmany_enumerator(const TSourceEnumerator& source, const TEnumeratorProjection& projector)
	: _source(source)
	, _projector(projector)
{
}

template <typename TSourceEnumerator, typename TEnumeratorProjection, typename TProjectedEnumeratorValue>
bool selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumeratorValue>::operator++()
{
	auto currentProjectedEnumeratorExhausted = !++_currentProjectedEnumerator;
	while (currentProjectedEnumeratorExhausted)
	{
		auto sourceIsExhausted = !++_source;
		if (sourceIsExhausted)
			return false;

		_currentProjectedEnumerator = _projector(*_source);
		currentProjectedEnumeratorExhausted = !++_currentProjectedEnumerator;
	}

	_currentProjectedValue = *_currentProjectedEnumerator;
	return true;
}

template <typename TSourceEnumerator, typename TEnumeratorProjection, typename TProjectedEnumeratorValue>
const typename selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumeratorValue>::value_type& selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumeratorValue>::operator*() const
{
	return _currentProjectedValue;
}
