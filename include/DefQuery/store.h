#pragma once

#include "enumerator.h"

namespace DefQuery
{
// ==============================================================================================

template <typename TValue, typename TDerived>
template <typename TContainer>
TContainer enumerator<TValue, TDerived>::store(
	const std::function<void(TContainer&, const TValue&)>& containerInserter)
{
	TContainer container;
	auto& self = static_cast<TDerived&>(*this);
	while(self.operator++())
		containerInserter(container, self.operator*());

	return container;
}
} // namespace DefQuery
