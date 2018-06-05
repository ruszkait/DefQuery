#pragma once

#include "enumerator.h"

namespace DefQuery
{
    // ==============================================================================================
    
    template <typename TValue, typename TDerived>
    std::list<TValue> enumerator<TValue, TDerived>::list()
    {
        std::list<TValue> result;
        auto& self = static_cast<TDerived&>(*this);
        while(self.operator++())
            result.emplace_back(self.operator*());
        
        return result;
    }
}
