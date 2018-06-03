#pragma once

#include "enumerator.h"

namespace DefQuery
{
    // ==============================================================================================

    template <typename TValue, typename TDerived>
    template <typename TFilter>
    bool enumerator<TValue, TDerived>::contains(TFilter filter)
    {
        while(this->move_next())
        {
            if (filter(this->current()))
                return true;
        }

        return false;
    }
}
