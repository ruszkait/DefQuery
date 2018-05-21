#include "enumerator.h"

template <typename TSourceEnumerator, typename TEnumeratorProjection, typename TProjectedEnumeratorValue>
class selectmany_enumerator : public enumerator<typename TProjectedEnumeratorValue, selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumeratorValue>>
{
public:
	using projected_enumerator_type = typename TProjectedValue = std::result_of<TEnumeratorProjection(value_type&)>::type;

	selectmany_enumerator(const TSourceEnumerator& source, const TEnumeratorProjection& projection);

	bool moveNext() override;
	const value_type& current() const override;

protected:
	enumerator_interface<value_type>* clone() const override;

private:
	TSourceEnumerator _source;
	TEnumeratorProjection _projector;
	projected_enumerator_type _currentProjectedEnumerator;
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
bool selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumeratorValue>::moveNext()
{
	auto isSourceNotExhausted = _source.moveNext();
	if (isSourceNotExhausted)
		_currentProjectedValue = _projector(_source.current());
	return isSourceNotExhausted;
}

template <typename TSourceEnumerator, typename TEnumeratorProjection, typename TProjectedEnumeratorValue>
const typename selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumeratorValue>::value_type& selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumeratorValue>::current() const
{
	return _currentProjectedValue;
}

template <typename TSourceEnumerator, typename TEnumeratorProjection, typename TProjectedEnumeratorValue>
enumerator_interface<typename selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumeratorValue>::value_type>* selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumeratorValue>::clone() const
{
	return new selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumeratorValue>(*this);
}