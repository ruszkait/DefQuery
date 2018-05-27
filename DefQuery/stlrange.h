#include "enumerator.h"
#include <iterator>

template <typename TSourceEnumerator>
class stlrange_adapter
{
public:
	using value_type = typename TSourceEnumerator::value_type;
	class stlrange_iterator : public std::iterator<std::forward_iterator_tag, value_type>
	{
	public:
		stlrange_iterator(stlrange_adapter& stlrange, bool hasEnded);

		stlrange_iterator(const stlrange_iterator& other) = default;
		stlrange_iterator(stlrange_iterator&& other) = default;
		stlrange_iterator& operator=(const stlrange_iterator& other) = default;
		stlrange_iterator& operator=(stlrange_iterator&& other) = default;

		const value_type& operator*() const;
		stlrange_iterator& operator++();
		bool operator==(const stlrange_iterator& other);
		bool operator!=(const stlrange_iterator& other);

	private:
		stlrange_adapter& _stlrangeAdapter;
		bool _hasEnded;
	};

	stlrange_adapter(const TSourceEnumerator& source);

	stlrange_adapter(const stlrange_adapter& other) = default;
	stlrange_adapter(stlrange_adapter&& other) = default;
	stlrange_adapter& operator=(const stlrange_adapter& other) = default;
	stlrange_adapter& operator=(stlrange_adapter&& other) = default;

	stlrange_iterator begin();
	stlrange_iterator end();

private:
	TSourceEnumerator _source;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
stlrange_adapter<TDerived> enumerator<TValue, TDerived>::stlrange()
{
	return stlrange_adapter<TDerived>(static_cast<TDerived&>(*this));
}

template<typename TSourceEnumerator>
stlrange_adapter<TSourceEnumerator>::stlrange_adapter(const TSourceEnumerator& source)
	: _source(source)
{
}

template<typename TSourceEnumerator>
typename stlrange_adapter<TSourceEnumerator>::stlrange_iterator stlrange_adapter<TSourceEnumerator>::begin()
{
	auto hasEnded = false;
	return stlrange_iterator(*this, hasEnded);
}

template<typename TSourceEnumerator>
typename stlrange_adapter<TSourceEnumerator>::stlrange_iterator stlrange_adapter<TSourceEnumerator>::end()
{
	auto hasEnded = true;
	return stlrange_iterator(*this, hasEnded);
}

template<typename TSourceEnumerator>
stlrange_adapter<TSourceEnumerator>::stlrange_iterator::stlrange_iterator(stlrange_adapter& stlrange, bool hasEnded)
	: _stlrangeAdapter(stlrange)
	, _hasEnded(hasEnded || !(++(_stlrangeAdapter._source)))
{
}

template<typename TSourceEnumerator>
const typename stlrange_adapter<TSourceEnumerator>::stlrange_iterator::value_type& stlrange_adapter<TSourceEnumerator>::stlrange_iterator::operator*() const
{
	return *(_stlrangeAdapter._source);
}

template<typename TSourceEnumerator>
typename stlrange_adapter<TSourceEnumerator>::stlrange_iterator& stlrange_adapter<TSourceEnumerator>::stlrange_iterator::operator++()
{
	_hasEnded = !(++(_stlrangeAdapter._source));
	return *this;
}

template<typename TSourceEnumerator>
bool stlrange_adapter<TSourceEnumerator>::stlrange_iterator::operator==(const stlrange_iterator& other)
{
	return _hasEnded == other._hasEnded;
}

template<typename TSourceEnumerator>
bool stlrange_adapter<TSourceEnumerator>::stlrange_iterator::operator!=(const stlrange_iterator& other)
{
	return !(*this == other);
}
