#pragma once

#include <DefQuery/enumerator.h>

namespace DefQuery
{
// ==============================================================================================

template <typename TValue, typename TDerived>
template <typename TFilter>
bool enumerator<TValue, TDerived>::all(const TFilter& filter)
{
	auto& self = static_cast<TDerived&>(*this);

	while(++self)
	{
		auto itemPassedTheFilter = filter(*self);
		if(!itemPassedTheFilter)
			return false;
	}

	return true;
}
} // namespace DefQuery
