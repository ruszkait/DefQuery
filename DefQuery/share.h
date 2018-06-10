#pragma once

#include "enumerator.h"
#include <memory>

namespace DefQuery
{
	template <typename TValue>
	class shared_enumerator : public enumerator<TValue, shared_enumerator<TValue>>
	{
	public:
        using TBaseClass = enumerator<TValue, shared_enumerator<TValue>>;
        
        shared_enumerator();
        
		shared_enumerator(const std::shared_ptr<enumerator_interface<TValue>>& source);

		shared_enumerator(const shared_enumerator& other) = default;
		shared_enumerator(shared_enumerator&& other) = default;
		shared_enumerator& operator=(const shared_enumerator& other) = default;
		shared_enumerator& operator=(shared_enumerator&& other) = default;

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
	shared_enumerator<TValue> enumerator<TValue, TDerived>::share() &&
	{
        auto instanceOnTheHeap = std::make_shared<TDerived>(std::move(static_cast<const TDerived&>(*this)));
        return shared_enumerator<TValue>(instanceOnTheHeap);
	}

    template <typename TValue, typename TDerived>
    shared_enumerator<TValue> enumerator<TValue, TDerived>::share() &
    {
        auto instanceOnTheHeap = std::make_shared<TDerived>(static_cast<const TDerived&>(*this));
        return shared_enumerator<TValue>(instanceOnTheHeap);
    }

    template<typename TValue>
    shared_enumerator<TValue>::shared_enumerator()
        : TBaseClass(true)
    {}
    
	template<typename TValue>
	shared_enumerator<TValue>::shared_enumerator(const std::shared_ptr<enumerator_interface<TValue>>& source)
        : TBaseClass(false)
		, _source(source)
	{}

	template<typename TValue>
	bool shared_enumerator<TValue>::operator++()
	{
        if (TBaseClass::exhausted())
            return TBaseClass::is_valid();
        
        TBaseClass::exhausted(!_source->move_next());
        return TBaseClass::is_valid();
	}

	template<typename TValue>
	const TValue& shared_enumerator<TValue>::operator*() const
	{
		return _source->current();
	}

    template<typename TValue>
    const TValue* shared_enumerator<TValue>::operator->() const
    {
        return &operator*();
    }
}
