#pragma once

#include "enumerator.h"

namespace DefQuery
{
	template <typename TSourceEnumerator, typename TRunningCondition>
	class takewhile_enumerator : public enumerator<typename TSourceEnumerator::value_type, takewhile_enumerator<TSourceEnumerator, TRunningCondition>>
	{
	public:
		using TBaseClass = enumerator<typename TSourceEnumerator::value_type, takewhile_enumerator<TSourceEnumerator, TRunningCondition>>;

		takewhile_enumerator();

		template <typename TSourceEnumeratorConstr, typename TRunningConditionConstr>
		takewhile_enumerator(TSourceEnumeratorConstr&& source, TRunningConditionConstr&& runningCondition);

		takewhile_enumerator(const takewhile_enumerator& other) = default;
		takewhile_enumerator(takewhile_enumerator&& other) = default;
		takewhile_enumerator& operator=(const takewhile_enumerator& other) = default;
		takewhile_enumerator& operator=(takewhile_enumerator&& other) = default;

		bool operator++();
		const typename TSourceEnumerator::value_type& operator*() const;
		const typename TSourceEnumerator::value_type* operator->() const;

	private:
		bool move_next() override { return this->operator++(); }
		const typename TSourceEnumerator::value_type& current() const override { return this->operator*(); }

		TSourceEnumerator _source;
		TRunningCondition _runningCondition;
	};

	// ==============================================================================================

	template <typename TValue, typename TDerived>
	template <typename TRunningCondition>
	takewhile_enumerator<TDerived, TRunningCondition> enumerator<TValue, TDerived>::takewhile(TRunningCondition&& runningCondition) &&
	{
		return takewhile_enumerator<TDerived, TRunningCondition>(std::move(static_cast<TDerived&>(*this)), std::forward<TRunningCondition>(runningCondition));
	}

	template <typename TValue, typename TDerived>
	template <typename TRunningCondition>
	takewhile_enumerator<TDerived, TRunningCondition> enumerator<TValue, TDerived>::takewhile(TRunningCondition&& runningCondition) &
	{
		return takewhile_enumerator<TDerived, TRunningCondition>(static_cast<TDerived&>(*this), std::forward<TRunningCondition>(runningCondition));
	}

	template<typename TSourceEnumerator, typename TRunningCondition>
	takewhile_enumerator<TSourceEnumerator, TRunningCondition>::takewhile_enumerator()
		: TBaseClass(true)
	{}

	template<typename TSourceEnumerator, typename TRunningCondition>
	template <typename TSourceEnumeratorConstr, typename TRunningConditionConstr>
	takewhile_enumerator<TSourceEnumerator, TRunningCondition>::takewhile_enumerator(TSourceEnumeratorConstr&& source, TRunningConditionConstr&& runningCondition)
		: TBaseClass(false)
		, _source(std::forward<TSourceEnumeratorConstr>(source))
		, _runningCondition(std::forward<TRunningConditionConstr>(runningCondition))
	{}

	template<typename TSourceEnumerator, typename TRunningCondition>
	bool takewhile_enumerator<TSourceEnumerator, TRunningCondition>::operator++()
	{
		if (TBaseClass::exhausted())
			return TBaseClass::is_valid();

		auto sourceExhausted = !(++_source);
		if (sourceExhausted)
			TBaseClass::exhausted(true);
		else
		{
			auto noMoreToTake = !_runningCondition(*_source);
			if (noMoreToTake)
				TBaseClass::exhausted(true);
		}

		return TBaseClass::is_valid();
	}

	template<typename TSourceEnumerator, typename TRunningCondition>
	const typename TSourceEnumerator::value_type& takewhile_enumerator<TSourceEnumerator, TRunningCondition>::operator*() const
	{
		return *_source;
	}

	template<typename TSourceEnumerator, typename TRunningCondition>
	const typename TSourceEnumerator::value_type* takewhile_enumerator<TSourceEnumerator, TRunningCondition>::operator->() const
	{
		return &operator*();
	}
}
