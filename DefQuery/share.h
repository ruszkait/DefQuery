#include "enumerator.h"

template <typename T>
class shared_enumerator : public enumerator<T, shared_enumerator<T>>
{
public:
	shared_enumerator(const shared_enumerator& other);
	shared_enumerator(enumerator_interface<T>* source);

	bool moveNext() override;
	Type& current() const override;

protected:
	enumerator_interface<Type>* clone() const override;

private:
	std::shared_ptr<enumerator_interface<T>> _source;
};

// ==============================================================================================

template <typename T, typename TDerived>
std::shared_ptr<shared_enumerator<T>> enumerator<T, TDerived>::share()
{
	auto cloneOnTheHeap = clone();
	auto wrappedClone = new shared_enumerator<T>(cloneOnTheHeap);
	return std::shared_ptr<shared_enumerator<T>>(wrappedClone);
}

template<typename T>
shared_enumerator<T>::shared_enumerator(const shared_enumerator& other)
	: _source(other._source)
{
}

template<typename T>
shared_enumerator<T>::shared_enumerator(enumerator_interface<T>* source)
	: _source(source)
{
}

template<typename T>
bool shared_enumerator<T>::moveNext()
{
	return _source->moveNext();
}

template<typename T>
typename shared_enumerator<T>::Type& shared_enumerator<T>::current() const
{
	return _source->current();
}

template<typename T>
enumerator_interface<typename shared_enumerator<T>::Type>* shared_enumerator<T>::clone() const
{
	return NULL;
}
