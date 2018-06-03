#pragma once

#include "enumerator.h"

namespace DefQuery
{
    // ==============================================================================================

    template <typename TValue, typename TDerived>
    template <typename TFilter>
    bool enumerator<TValue, TDerived>::contains(TFilter filter)
    {
        // Use the derived type directly to avoid using virtual functions
        // to progress with source enumerator consumption
        auto& self = dynamic_cast<TDerived&>(*this);
        while(self.operator++())
        {
            if (filter(self.operator*()))
                return true;
        }

        return false;
    }
}
