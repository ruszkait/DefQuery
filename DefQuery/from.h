#include "enumerator.h"

template <typename TContainer>
class from_enumerator : public enumerator<typename TContainer::value_type, from_enumerator<TContainer>>
{
public:
	from_enumerator(TContainer& container);

	bool moveNext() override;
	Type& current() const override;

private:
	typename TContainer::iterator _current;
	typename TContainer::iterator _end;
	bool _firstMoveNext;
};

template <typename TContainer>
//from_enumerator<TContainer> from(TContainer&& container)
from_enumerator<TContainer> from(TContainer& container)
{
	return from_enumerator<TContainer>(container);
//	return from_enumerator<std::remove_reference<TContainer>>(std::forward<TContainer&&>(container));
//	return from_enumerator<TContainer>(container);
}

// ==============================================================================================

template<typename TContainer>
from_enumerator<TContainer>::from_enumerator(TContainer & container)
	: _current(container.begin())
	, _end(container.end())
	, _firstMoveNext(true)
{
}

template<typename TContainer>
bool from_enumerator<TContainer>::moveNext()
{
	if (_firstMoveNext)
		_firstMoveNext = false;
	else
		++_current;

	auto containerEndHasNotBeenReached = _current != _end;
	return containerEndHasNotBeenReached;
}

template<typename TContainer>
typename from_enumerator<TContainer>::Type& from_enumerator<TContainer>::current() const
{
	return *_current;
}
