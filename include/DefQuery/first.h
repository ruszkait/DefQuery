#pragma once

#include "enumerator.h"

namespace DefQuery
{
// ==============================================================================================

template <typename TValue, typename TDerived>
TValue enumerator<TValue, TDerived>::first()
{
	auto& self = static_cast<TDerived&>(*this);
	if(!self.operator++())
		throw std::runtime_error("cannot take first element of an empty enumeration");

	return self.operator*();
}

template <typename TValue, typename TDerived>
TValue enumerator<TValue, TDerived>::first_or_default()
{
	auto& self = static_cast<TDerived&>(*this);
	if(!self.operator++())
		return TValue();
	;

	return self.operator*();
}
} // namespace DefQuery
