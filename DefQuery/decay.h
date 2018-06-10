#pragma once

#include "enumerator.h"
#include <memory>

namespace DefQuery
{
	template <typename TValue>
	class decayed_enumerator : public enumerator<TValue, decayed_enumerator<TValue>>
	{
	public:
        using TBaseClass = enumerator<TValue, decayed_enumerator<TValue>>;
        
		decayed_enumerator();
        
		decayed_enumerator(const std::shared_ptr<enumerator_interface<TValue>>& source);

		decayed_enumerator(const decayed_enumerator& other) = default;
		decayed_enumerator(decayed_enumerator&& other) = default;
		decayed_enumerator& operator=(const decayed_enumerator& other) = default;
		decayed_enumerator& operator=(decayed_enumerator&& other) = default;

		bool operator++();
		const TValue& operator*() const;
        const TValue* operator->() const;

	private:
		bool move_next() override { return this->operator++(); }
		const TValue& current() const override { return this->operator*(); }

		std::shared_ptr<enumerator_interface<TValue>> _source;
	};

	// ==============================================================================================

	template <typename TValue, typename TDerived>
	decayed_enumerator<TValue> enumerator<TValue, TDerived>::decay() &&
	{
        auto instanceOnTheHeap = std::make_shared<TDerived>(std::move(static_cast<const TDerived&>(*this)));
        return decayed_enumerator<TValue>(instanceOnTheHeap);
	}

    template <typename TValue, typename TDerived>
	decayed_enumerator<TValue> enumerator<TValue, TDerived>::decay() &
    {
        auto instanceOnTheHeap = std::make_shared<TDerived>(static_cast<const TDerived&>(*this));
        return decayed_enumerator<TValue>(instanceOnTheHeap);
    }

    template<typename TValue>
	decayed_enumerator<TValue>::decayed_enumerator()
        : TBaseClass(true)
    {}
    
	template<typename TValue>
	decayed_enumerator<TValue>::decayed_enumerator(const std::shared_ptr<enumerator_interface<TValue>>& source)
        : TBaseClass(false)
		, _source(source)
	{}

	template<typename TValue>
	bool decayed_enumerator<TValue>::operator++()
	{
        if (TBaseClass::exhausted())
            return TBaseClass::is_valid();
        
        TBaseClass::exhausted(!_source->move_next());
        return TBaseClass::is_valid();
	}

	template<typename TValue>
	const TValue& decayed_enumerator<TValue>::operator*() const
	{
		return _source->current();
	}

    template<typename TValue>
    const TValue* decayed_enumerator<TValue>::operator->() const
    {
        return &operator*();
    }
}
