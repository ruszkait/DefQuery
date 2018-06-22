#pragma once

#include <array>
#include <utility>
#include "enumerator.h"

namespace DefQuery
{
	template <typename TLeftEnumerator, typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
	class join_enumerator : public enumerator<typename std::result_of<TMerging(const typename TLeftEnumerator::value_type&, const typename TRightEnumerator::value_type&)>::type, join_enumerator<TLeftEnumerator, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>>
	{
	public:
		using TMergedValue = typename std::result_of<TMerging(const typename TLeftEnumerator::value_type&, const typename TRightEnumerator::value_type&)>::type;
		using TBaseClass = enumerator<typename std::result_of<TMerging(const typename TLeftEnumerator::value_type&, const typename TRightEnumerator::value_type&)>::type, join_enumerator<TLeftEnumerator, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>>;

		join_enumerator();

		template <typename TLeftEnumeratorConstr, typename TRightEnumeratorConstr, typename TKeyProjectionLeftConstr, typename TKeyProjectionRightConstr, typename TMergingConstr>
		join_enumerator(TLeftEnumeratorConstr&& sourceLeft, TRightEnumeratorConstr&& sourceRight, TKeyProjectionLeftConstr&& projectionKeyLeft, TKeyProjectionRightConstr&& projectionKeyRight, TMergingConstr&& merging);
		~join_enumerator();

		join_enumerator(const join_enumerator& other) = default;
		join_enumerator(join_enumerator&& other) = default;
		join_enumerator& operator=(const join_enumerator& other) = default;
		join_enumerator& operator=(join_enumerator&& other) = default;

		bool operator++();
		const TMergedValue& operator*() const;
		const TMergedValue* operator->() const;

	private:
		bool move_next() override { return this->operator++(); }
		const TMergedValue& current() const override { return this->operator*(); }
		void free_current_merged_value();

		TLeftEnumerator _sourceLeft;
		TRightEnumerator _sourceRightMaster;
		TRightEnumerator _sourceRightCurrent;

		TKeyProjectionLeft _keyProjectionLeft;
		TKeyProjectionRight _keyProjectionRight;
		TMerging _merging;
		// FIXME: Poor man's std::optional
		bool _currentMergedValueDefined;
		std::array<char, sizeof(TMergedValue)> _currentMergedValue;
	};

	// ==============================================================================================

	template <typename TValue, typename TDerived>
	template <typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
	join_enumerator<TDerived, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging> enumerator<TValue, TDerived>::join(TRightEnumerator&& sourceRight, TKeyProjectionLeft&& keyProjectionLeft, TKeyProjectionRight&& keyProjectionRight, TMerging&& merging) &&
	{
		return join_enumerator<TDerived, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>(std::move(static_cast<TDerived&>(*this)),
			std::forward<TRightEnumerator>(sourceRight),
			std::forward<TKeyProjectionLeft>(keyProjectionLeft),
			std::forward<TKeyProjectionRight>(keyProjectionRight),
			std::forward<TMerging>(merging));
	}

	template <typename TValue, typename TDerived>
	template <typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
	join_enumerator<TDerived, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging> enumerator<TValue, TDerived>::join(TRightEnumerator&& sourceRight, TKeyProjectionLeft&& keyProjectionLeft, TKeyProjectionRight&& keyProjectionRight, TMerging&& merging) &
	{
		return join_enumerator<TDerived, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>(std::move(static_cast<TDerived&>(*this)),
			std::forward<TRightEnumerator>(sourceRight),
			std::forward<TKeyProjectionLeft>(keyProjectionLeft),
			std::forward<TKeyProjectionRight>(keyProjectionRight),
			std::forward<TMerging>(merging));
	}

	template <typename TLeftEnumerator, typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
	join_enumerator<TLeftEnumerator, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>::join_enumerator()
		: TBaseClass(true)
	{}

	template <typename TLeftEnumerator, typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
	template <typename TLeftEnumeratorConstr, typename TRightEnumeratorConstr, typename TKeyProjectionLeftConstr, typename TKeyProjectionRightConstr, typename TMergingConstr>
	join_enumerator<TLeftEnumerator, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>::join_enumerator(TLeftEnumeratorConstr&& sourceLeft, TRightEnumeratorConstr&& sourceRight, TKeyProjectionLeftConstr&& projectionKeyLeft, TKeyProjectionRightConstr&& projectionKeyRight, TMergingConstr&& merging)
		: TBaseClass(false)
		, _sourceLeft(std::forward<TLeftEnumeratorConstr>(sourceLeft))
		, _sourceRightMaster(std::forward<TRightEnumerator>(sourceRight))
		, _keyProjectionLeft(std::forward<TKeyProjectionLeftConstr>(projectionKeyLeft))
		, _keyProjectionRight(std::forward<TKeyProjectionRightConstr>(projectionKeyRight))
		, _merging(std::forward<TMergingConstr>(merging))
		, _currentMergedValueDefined(false)
	{}

	template <typename TLeftEnumerator, typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
	join_enumerator<TLeftEnumerator, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>::~join_enumerator()
	{
		free_current_merged_value();
	}

	template <typename TLeftEnumerator, typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
	void join_enumerator<TLeftEnumerator, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>::free_current_merged_value()
	{
		if (!_currentMergedValueDefined)
			return;

		reinterpret_cast<TMergedValue*>(_currentMergedValue.data())->~TMergedValue();
		_currentMergedValueDefined = false;
	}

	template <typename TLeftEnumerator, typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
	bool join_enumerator<TLeftEnumerator, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>::operator++()
	{
		while (++_sourceLeft)
		{
			if (!++_sourceRightCurrent)
			{
				_sourceRightCurrent = _sourceRightMaster;
				if (!++_sourceRightCurrent)
					return false;
			}

			auto leftKey = _keyProjectionLeft(*_sourceLeft);
			auto rightKey = _keyProjectionRight(*_sourceRightCurrent);

			auto keysInnerJoinMatch = leftKey == rightKey;
			if (keysInnerJoinMatch)
			{
				free_current_merged_value();
				new (_currentMergedValue.data()) TMergedValue(_merging(*_sourceLeft, *_sourceRightCurrent));
				_currentMergedValueDefined = true;
				return true;
			}
		}

		return false;
	}

	template <typename TLeftEnumerator, typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
	const typename join_enumerator<TLeftEnumerator, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>::TMergedValue& join_enumerator<TLeftEnumerator, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>::operator*() const
	{
		return *reinterpret_cast<const TMergedValue*>(_currentMergedValue.data());
	}

	template <typename TLeftEnumerator, typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
	const typename join_enumerator<TLeftEnumerator, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>::TMergedValue* join_enumerator<TLeftEnumerator, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging>::operator->() const
	{
		return &operator*();
	}
}
