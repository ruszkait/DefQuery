#pragma once 

#include "enumerator.h"

namespace DefQuery
{
	// ==============================================================================================

	template <typename TValue, typename TDerived>
	template <typename TAccumlatorInitializer, typename TFolding, typename TAccumlator>
	TAccumlator enumerator<TValue, TDerived>::aggregate(TFolding folder, TAccumlatorInitializer accumlatorInitializer)
    {
        // Use the derived type directly to avoid using virtual functions
        // to progress with source enumerator consumption
        auto& self = dynamic_cast<TDerived&>(*this);

        if (!self.operator++())
			return TAccumlator();

        TAccumlator accumlator = accumlatorInitializer(self.operator*());

        while (self.operator++())
		{
            folder(accumlator, self.operator*());
		}
		return accumlator;
	}
}
