#pragma once

#include <array>
#include "enumerator.h"

namespace DefQuery
{
	template <typename TSourceEnumerator, typename TProjection>
	class select_enumerator : public enumerator<typename std::result_of<TProjection(const typename TSourceEnumerator::value_type&)>::type, select_enumerator<TSourceEnumerator, TProjection>>
	{
	public:
		using TProjectedValue = typename std::result_of<TProjection(const typename TSourceEnumerator::value_type&)>::type;
		using TBaseClass = enumerator<typename std::result_of<TProjection(const typename TSourceEnumerator::value_type&)>::type, select_enumerator<TSourceEnumerator, TProjection>>;

		select_enumerator();

		template <typename TSourceEnumeratorConstr, typename TProjectionConstr>
		select_enumerator(TSourceEnumeratorConstr&& source, TProjectionConstr&& projection);
		~select_enumerator();

		select_enumerator(const select_enumerator& other) = default;
		select_enumerator(select_enumerator&& other) = default;
		select_enumerator& operator=(const select_enumerator& other) = default;
		select_enumerator& operator=(select_enumerator&& other) = default;

		bool operator++();
		const TProjectedValue& operator*() const;
		const TProjectedValue* operator->() const;

	private:
		bool move_next() override { return this->operator++(); }
		const TProjectedValue& current() const override { return this->operator*(); }
		void free_current_projected_value();

		TSourceEnumerator _source;
		TProjection _projector;
		// FIXME: Poor man's std::optional
		bool _currentProjectedValueDefined;
		std::array<char, sizeof(TProjectedValue)> _currentProjectedValue;
	};

	// ==============================================================================================

	template <typename TValue, typename TDerived>
	template <typename TProjection>
	select_enumerator<TDerived, TProjection> enumerator<TValue, TDerived>::select(TProjection&& projector) &&
	{
		return select_enumerator<TDerived, TProjection>(std::move(static_cast<TDerived&>(*this)), std::forward<TProjection>(projector));
	}

	template <typename TValue, typename TDerived>
	template <typename TProjection>
	select_enumerator<TDerived, TProjection> enumerator<TValue, TDerived>::select(TProjection&& projector) &
	{
		return select_enumerator<TDerived, TProjection>(static_cast<TDerived&>(*this), std::forward<TProjection>(projector));
	}

	template <typename TSourceEnumerator, typename TProjection>
	select_enumerator<TSourceEnumerator, TProjection>::select_enumerator()
		: TBaseClass(true)
	{}

	template <typename TSourceEnumerator, typename TProjection>
	template <typename TSourceEnumeratorConstr, typename TProjectionConstr>
	select_enumerator<TSourceEnumerator, TProjection>::select_enumerator(TSourceEnumeratorConstr&& source, TProjectionConstr&& projector)
		: TBaseClass(false)
		, _source(std::forward<TSourceEnumeratorConstr>(source))
		, _projector(std::forward<TProjectionConstr>(projector))
		, _currentProjectedValueDefined(false)
	{}

	template <typename TSourceEnumerator, typename TProjection>
	select_enumerator<TSourceEnumerator, TProjection>::~select_enumerator()
	{
		free_current_projected_value();
	}

	template <typename TSourceEnumerator, typename TProjection>
	void select_enumerator<TSourceEnumerator, TProjection>::free_current_projected_value()
	{
		if (!_currentProjectedValueDefined)
			return;

		reinterpret_cast<TProjectedValue*>(_currentProjectedValue.data())->~TProjectedValue();
		_currentProjectedValueDefined = false;
	}

	template <typename TSourceEnumerator, typename TProjection>
	bool select_enumerator<TSourceEnumerator, TProjection>::operator++()
	{
		if (TBaseClass::exhausted())
			return TBaseClass::is_valid();

		auto isSourceNotExhausted = ++_source;
		if (isSourceNotExhausted)
		{
			free_current_projected_value();

			new (_currentProjectedValue.data()) TProjectedValue(_projector(*_source));
			_currentProjectedValueDefined = true;
		}

		TBaseClass::exhausted(!isSourceNotExhausted);
		return TBaseClass::is_valid();
	}

	template <typename TSourceEnumerator, typename TProjection>
	const typename select_enumerator<TSourceEnumerator, TProjection>::TProjectedValue& select_enumerator<TSourceEnumerator, TProjection>::operator*() const
	{
		return *reinterpret_cast<const TProjectedValue*>(_currentProjectedValue.data());
	}

	template <typename TSourceEnumerator, typename TProjection>
	const typename select_enumerator<TSourceEnumerator, TProjection>::TProjectedValue* select_enumerator<TSourceEnumerator, TProjection>::operator->() const
	{
		return &operator*();
	}
}
