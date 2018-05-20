#include "enumerator.h"

template <typename TSourceEnumerator, typename TFilter>
class where_enumerator : public enumerator<typename TSourceEnumerator::value_type, where_enumerator<TSourceEnumerator, TFilter>>
{
public:
	where_enumerator(const TSourceEnumerator& source, const TFilter& filter);

	bool moveNext() override;
	value_type& current() const override;

protected:
	enumerator_interface<value_type>* clone() const override;

private:
	TSourceEnumerator _source;
	TFilter _filter;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
template <typename TFilter>
where_enumerator<TDerived, TFilter> enumerator<TValue, TDerived>::where(const TFilter& filter)
{
	return where_enumerator<TDerived, TFilter>(static_cast<TDerived&>(*this), filter);
}

template<typename TSourceEnumerator, typename TFilter>
where_enumerator<TSourceEnumerator, TFilter>::where_enumerator(const TSourceEnumerator& source, const TFilter& filter)
	: _source(source)
	, _filter(filter)
{
}

template<typename TSourceEnumerator, typename TFilter>
bool where_enumerator<TSourceEnumerator, TFilter>::moveNext()
{
	while (_source.moveNext())
	{
		auto itemPassedTheFilter = _filter(_source.current());
		if (itemPassedTheFilter)
			return true;
	}

	return false;
}

template<typename TSourceEnumerator, typename TFilter>
typename where_enumerator<TSourceEnumerator, TFilter>::value_type& where_enumerator<TSourceEnumerator, TFilter>::current() const
{
	return _source.current();
}

template<typename TSourceEnumerator, typename TFilter>
enumerator_interface<typename where_enumerator<TSourceEnumerator, TFilter>::value_type>* where_enumerator<TSourceEnumerator, TFilter>::clone() const
{
	return new where_enumerator<TSourceEnumerator, TFilter>(*this);
}