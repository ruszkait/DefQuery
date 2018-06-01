#pragma once 

#include "enumerator.h"

namespace DefQuery
{
	// ==============================================================================================

	template <typename TValue, typename TDerived>
	template <typename TAccumlatorInitializer, typename TFolding, typename TAccumlator>
	TAccumlator enumerator<TValue, TDerived>::aggregate(TFolding folder, TAccumlatorInitializer accumlatorInitializer)
    {
        if (!this->move_next())
			return TAccumlator();

        TAccumlator accumlator = accumlatorInitializer(this->current());

		while (this->move_next())
		{
			folder(accumlator, this->current());
		}
		return accumlator;
	}
}
