#pragma once 

#include <array>
#include "enumerator.h"

namespace DefQuery
{
	template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
	class select_enumerator : public enumerator<TProjectedValue, select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>>
	{
	public:
        template <typename TSourceEnumeratorConstr>
		select_enumerator(TSourceEnumeratorConstr&& source, const TProjection& projection);
        ~select_enumerator();

		select_enumerator(const select_enumerator& other) = default;
		select_enumerator(select_enumerator&& other) = default;
		select_enumerator& operator=(const select_enumerator& other) = default;
		select_enumerator& operator=(select_enumerator&& other) = default;

		bool operator++();
		const TProjectedValue& operator*() const;

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
	template <typename TProjection, typename TProjectedValue>
	select_enumerator<TDerived, TProjection, TProjectedValue> enumerator<TValue, TDerived>::select(const TProjection& projector)
	{
        return select_enumerator<TDerived, TProjection, TProjectedValue>(std::move(static_cast<TDerived&>(*this)), projector);
	}

	template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
    template <typename TSourceEnumeratorConstr>
	select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>::select_enumerator(TSourceEnumeratorConstr&& source, const TProjection& projector)
        : _source(std::forward<TSourceEnumeratorConstr>(source))
		, _projector(projector)
        , _currentProjectedValueDefined(false)
    {}
    
    template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
    select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>::~select_enumerator()
    {
        free_current_projected_value();
    }

    template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
    void select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>::free_current_projected_value()
    {
        if (!_currentProjectedValueDefined)
            return;
        
        reinterpret_cast<TProjectedValue*>(_currentProjectedValue.data())->~TProjectedValue();
        _currentProjectedValueDefined = false;
    }
    
	template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
	bool select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>::operator++()
	{
		auto isSourceNotExhausted = ++_source;
		if (isSourceNotExhausted)
        {
            free_current_projected_value();
            
			new (_currentProjectedValue.data()) TProjectedValue(_projector(*_source));
            _currentProjectedValueDefined = true;
        }
		return isSourceNotExhausted;
	}

	template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
	const TProjectedValue& select_enumerator<TSourceEnumerator, TProjection, TProjectedValue>::operator*() const
	{
		return *reinterpret_cast<const TProjectedValue*>(_currentProjectedValue.data());
	}
}
