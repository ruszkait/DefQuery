#pragma once 

#include <exception>
#include "enumerator.h"

namespace DefQuery
{
	// ==============================================================================================

	template <typename TValue, typename TDerived>
	template <typename TAccumlatorInitializer, typename TFolding>
	typename std::result_of<TAccumlatorInitializer(const TValue&)>::type enumerator<TValue, TDerived>::aggregate(const TFolding& folder, const TAccumlatorInitializer& accumlatorInitializer)
	{
		// Use the derived type directly to avoid using virtual functions
		// to progress with source enumerator consumption
		auto& self = static_cast<TDerived&>(*this);

		if (!self.operator++())
			throw std::runtime_error("cannot aggregatae an empty enumeration");

		using TAccumlator = typename std::result_of<TAccumlatorInitializer(const TValue&)>::type;
		TAccumlator accumlator = accumlatorInitializer(self.operator*());

		while (self.operator++())
		{
			folder(accumlator, self.operator*());
		}
		return accumlator;
	}
}
