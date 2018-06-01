#pragma once 

#include "enumerator.h"
#include <memory>

namespace DefQuery
{
	template <typename TValue>
	class shared_enumerator : public enumerator<TValue, shared_enumerator<TValue>>
	{
	public:
		shared_enumerator(enumerator_interface<TValue>* source);

		shared_enumerator(const shared_enumerator& other) = default;
		shared_enumerator(shared_enumerator&& other) = default;
		shared_enumerator& operator=(const shared_enumerator& other) = default;
		shared_enumerator& operator=(shared_enumerator&& other) = default;

		bool operator++();
		const TValue& operator*() const;

	private:
		bool move_next() override { return this->operator++(); }
		const TValue& current() const override { return this->operator*(); }

		std::shared_ptr<enumerator_interface<TValue>> _source;
	};

	// ==============================================================================================

	template <typename TValue, typename TDerived>
	shared_enumerator<TValue> enumerator<TValue, TDerived>::share()
	{
		auto cloneOnTheHeap = clone();
		return shared_enumerator<TValue>(cloneOnTheHeap);
	}

	template<typename TValue>
	shared_enumerator<TValue>::shared_enumerator(enumerator_interface<TValue>* source)
		: _source(source)
	{
	}

	template<typename TValue>
	bool shared_enumerator<TValue>::operator++()
	{
		return _source->move_next();
	}

	template<typename TValue>
	const TValue& shared_enumerator<TValue>::operator*() const
	{
		return _source->current();
	}
}
