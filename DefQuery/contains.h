#pragma once

#include "enumerator.h"

namespace DefQuery
{
	// ==============================================================================================

	template <typename TValue, typename TDerived>
	bool enumerator<TValue, TDerived>::contains(const TValue& searched)
	{
		// Use the derived type directly to avoid using virtual functions
		// to progress with source enumerator consumption
		auto& self = static_cast<TDerived&>(*this);
		while (self.operator++())
		{
			if (searched == self.operator*())
				return true;
		}

		return false;
	}

	template <typename TValue, typename TDerived>
	template <typename TFilter>
	bool enumerator<TValue, TDerived>::contains(const TFilter& filter)
	{
		// Use the derived type directly to avoid using virtual functions
		// to progress with source enumerator consumption
		auto& self = static_cast<TDerived&>(*this);
		while (self.operator++())
		{
			if (filter(self.operator*()))
				return true;
		}

		return false;
	}
}
