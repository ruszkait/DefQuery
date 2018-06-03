#pragma once 

#include "enumerator.h"

namespace DefQuery
{
	// ==============================================================================================

	template <typename TValue, typename TDerived>
	std::size_t enumerator<TValue, TDerived>::count()
	{
		std::size_t counter = 0;

		while(this->move_next())
			++counter;

		return counter;
	}
}
