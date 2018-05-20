#include "enumerator.h"

template <typename TSourceEnumerator, typename TFilter>
class where_enumerator : public enumerator<typename TSourceEnumerator::Type, where_enumerator<TSourceEnumerator, TFilter>>
{
public:
	where_enumerator(TSourceEnumerator& source, TFilter filter);

	bool moveNext() override;
	Type& current() const override;

private:
	TSourceEnumerator _source;
	TFilter _filter;
};

// ==============================================================================================

template <typename T, typename TDerived>
template <typename TFilter>
enumerator<T, TDerived>::where<TFilter>(TFilter filter)
{
	return where_enumerator(static_cast<TDerived&>(*this), filter);
}

template<typename TSourceEnumerator, typename TFilter>
where_enumerator<T, TSourceEnumerator, TFilter>::where_enumerator(TSourceEnumeratorType&& source, TFilter filter)
	: _source(source)
	, _filter(filter)
{
}
