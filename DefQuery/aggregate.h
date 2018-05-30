#include "enumerator.h"

namespace DefQuery
{
	// ==============================================================================================

	template <typename TValue, typename TDerived>
	template <typename TAccumlatorInitializer, typename TFolding, typename TAccumlator>
	TAccumlator enumerator<TValue, TDerived>::aggregate(TFolding folder, TAccumlatorInitializer accumlatorInitializer)
	{
		if (!move_next())
			return TAccumlator();

		TAccumlator accumlator = accumlatorInitializer(current());

		while (move_next())
		{
			folder(accumlator, current());
		}
		return accumlator;
	}
}