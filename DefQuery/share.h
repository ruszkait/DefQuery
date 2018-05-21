#include "enumerator.h"

#include <memory>

template <typename TValue>
class shared_enumerator : public enumerator<TValue, shared_enumerator<TValue>>
{
public:
	shared_enumerator(const shared_enumerator& other);
	shared_enumerator(enumerator_interface<TValue>* source);

	bool operator++() override;
	const value_type& operator*() const override;

private:
	std::shared_ptr<enumerator_interface<value_type>> _source;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
shared_enumerator<TValue> enumerator<TValue, TDerived>::share()
{
	auto cloneOnTheHeap = clone();
	return shared_enumerator<TValue>(cloneOnTheHeap);
}

template<typename TValue>
shared_enumerator<TValue>::shared_enumerator(const shared_enumerator& other)
	: _source(other._source)
{
}

template<typename TValue>
shared_enumerator<TValue>::shared_enumerator(enumerator_interface<TValue>* source)
	: _source(source)
{
}

template<typename TValue>
bool shared_enumerator<TValue>::operator++()
{
	return ++(*_source);
}

template<typename TValue>
const typename shared_enumerator<TValue>::value_type& shared_enumerator<TValue>::operator*() const
{
	return **_source;
}
