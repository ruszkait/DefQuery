#include "enumerator.h"

template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
class select_enumerator : public enumerator<typename TProjectedValue, select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>>
{
public:
	select_enumerator(const TSourceEnumerator& source, const TProjection& projection);

	bool moveNext() override;
	const value_type& current() const override;

protected:
	enumerator_interface<value_type>* clone() const override;

private:
	TSourceEnumerator _source;
	TProjection _projector;
	TProjectedValue _currentProjectedValue;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
template <typename TProjection, typename TProjectedValue>
select_enumerator<TDerived, TProjection, TProjectedValue> enumerator<TValue, TDerived>::select(const TProjection& projector)
{
	return select_enumerator<TDerived, TProjection, TProjectedValue>(static_cast<TDerived&>(*this), projector);
}

template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>::select_enumerator(const TSourceEnumerator& source, const TProjection& projector)
	: _source(source)
	, _projector(projector)
{
}

template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
bool select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>::moveNext()
{
	auto isSourceNotExhausted = _source.moveNext();
	if (isSourceNotExhausted)
		_currentProjectedValue = _projector(_source.current());
	return isSourceNotExhausted;
}

template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
const typename select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>::value_type& select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>::current() const
{
	return _currentProjectedValue;
}

template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
enumerator_interface<typename select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>::value_type>* select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>::clone() const
{
	return new select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>(*this);
}