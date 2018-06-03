#pragma once 

#include "enumerator.h"

namespace DefQuery
{
	// ==============================================================================================

	template <typename TValue, typename TDerived>
	std::size_t enumerator<TValue, TDerived>::count()
	{
		std::size_t counter = 0;

        // Use the derived type directly to avoid using virtual functions
        // to progress with source enumerator consumption
        auto& self = static_cast<TDerived&>(*this);
        while(self.operator++())
			++counter;

		return counter;
	}
}
