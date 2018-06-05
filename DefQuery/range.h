#pragma once

#include <iterator>
#include "enumerator.h"

namespace DefQuery
{
    template <typename TValue>
    class range_enumerator : public enumerator<TValue, range_enumerator<TValue>>
    {
    public:
        range_enumerator(TValue begin, TValue end);
        
        range_enumerator(const range_enumerator& other) = default;
        range_enumerator(range_enumerator&& other) = default;
        range_enumerator& operator=(const range_enumerator& other) = default;
        range_enumerator& operator=(range_enumerator&& other) = default;
        
        range_enumerator step(std::size_t step);
        
        bool operator++();
        const TValue& operator*() const;
        
    private:
        bool move_next() override { return this->operator++(); }
        const TValue& current() const override { return this->operator*(); }
        
        TValue _current;
        TValue _end;
        std::size_t _step;
        bool _firstMoveNext;
    };
    
    // ==============================================================================================
    
    template <typename TValue>
    range_enumerator<TValue> range(TValue begin, TValue end)
    {
        return range_enumerator<TValue>(begin, end);
    }

    template<typename TValue>
    range_enumerator<TValue>::range_enumerator(TValue begin, TValue end)
        : _current(begin)
        , _end(end)
        , _step(1)
        , _firstMoveNext(true)
    {
    }

    template<typename TValue>
    range_enumerator<TValue> range_enumerator<TValue>::step(std::size_t step)
    {
        _step = step;
        return *this;
    }
    
    template<typename TValue>
    bool range_enumerator<TValue>::operator++()
    {
        auto containerEndHasBeenReached = _current == _end;
        if (containerEndHasBeenReached)
            return false;
        
        if (_firstMoveNext)
            _firstMoveNext = false;
        else
            for (auto stepsLeft = _step; stepsLeft > 0 && _current != _end; --stepsLeft)
                ++_current;
        
        auto containerEndHasNotBeenReached = _current != _end;
        return containerEndHasNotBeenReached;
    }
    
    template<typename TValue>
    const TValue& range_enumerator<TValue>::operator*() const
    {
        return _current;
    }
}
