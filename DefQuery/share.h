#include "enumerator.h"

template <typename TValue>
class shared_enumerator : public enumerator<TValue, shared_enumerator<TValue>>
{
public:
	shared_enumerator(const shared_enumerator& other);
	shared_enumerator(enumerator_interface<TValue>* source);

	bool moveNext() override;
	value_type& current() const override;

protected:
	enumerator_interface<value_type>* clone() const override;

private:
	std::shared_ptr<enumerator_interface<value_type>> _source;
};

// ==============================================================================================

template <typename TValue, typename TDerived>
std::shared_ptr<shared_enumerator<TValue>> enumerator<TValue, TDerived>::share()
{
	auto cloneOnTheHeap = clone();
	auto wrappedClone = new shared_enumerator<TValue>(cloneOnTheHeap);
	return std::shared_ptr<shared_enumerator<TValue>>(wrappedClone);
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
bool shared_enumerator<TValue>::moveNext()
{
	return _source->moveNext();
}

template<typename TValue>
typename shared_enumerator<TValue>::value_type& shared_enumerator<TValue>::current() const
{
	return _source->current();
}

template<typename TValue>
enumerator_interface<typename shared_enumerator<TValue>::value_type>* shared_enumerator<TValue>::clone() const
{
	return nullptr;
}
