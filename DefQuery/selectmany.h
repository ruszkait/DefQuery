#pragma once 

#include "enumerator.h"

namespace DefQuery
{
	template <typename TSourceEnumerator, typename TEnumeratorProjection, typename TProjectedEnumerator>
    class selectmany_enumerator : public enumerator<typename TProjectedEnumerator::value_type, selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumerator>>
	{
	public:
        template <typename TSourceEnumeratorConstr>
		selectmany_enumerator(TSourceEnumeratorConstr&& source, const TEnumeratorProjection& projection);

		selectmany_enumerator(const selectmany_enumerator& other) = default;
		selectmany_enumerator(selectmany_enumerator&& other) = default;
		selectmany_enumerator& operator=(const selectmany_enumerator& other) = default;
		selectmany_enumerator& operator=(selectmany_enumerator&& other) = default;

		bool operator++();
		const typename TProjectedEnumerator::value_type& operator*() const;

	private:
		bool move_next() override { return this->operator++(); }
		const typename TProjectedEnumerator::value_type& current() const override { return this->operator*(); }

		TSourceEnumerator _source;
		TEnumeratorProjection _projector;
		TProjectedEnumerator _currentProjectedEnumerator;
        typename TProjectedEnumerator::value_type _currentProjectedValue;
	};

	// ==============================================================================================

	template <typename TValue, typename TDerived>
	template <typename TEnumeratorProjection, typename TProjectedEnumerator>
	selectmany_enumerator<TDerived, TEnumeratorProjection, TProjectedEnumerator> enumerator<TValue, TDerived>::selectmany(const TEnumeratorProjection& projector)
	{
        return selectmany_enumerator<TDerived, TEnumeratorProjection, TProjectedEnumerator>(std::move(static_cast<TDerived&>(*this)), projector);
	}

	template <typename TSourceEnumerator, typename TEnumeratorProjection, typename TProjectedEnumerator>
    template <typename TSourceEnumeratorConstr>
	selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumerator>::selectmany_enumerator(TSourceEnumeratorConstr&& source, const TEnumeratorProjection& projector)
        : _source(std::forward<TSourceEnumeratorConstr>(source))
		, _projector(projector)
	{
	}

	template <typename TSourceEnumerator, typename TEnumeratorProjection, typename TProjectedEnumerator>
	bool selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumerator>::operator++()
	{
		auto currentProjectedEnumeratorExhausted = !++_currentProjectedEnumerator;
		while (currentProjectedEnumeratorExhausted)
		{
			auto sourceIsExhausted = !++_source;
			if (sourceIsExhausted)
				return false;

			_currentProjectedEnumerator = _projector(*_source);
			currentProjectedEnumeratorExhausted = !++_currentProjectedEnumerator;
		}

		_currentProjectedValue = *_currentProjectedEnumerator;
		return true;
	}

	template <typename TSourceEnumerator, typename TEnumeratorProjection, typename TProjectedEnumerator>
    const typename TProjectedEnumerator::value_type& selectmany_enumerator<TSourceEnumerator, TEnumeratorProjection, TProjectedEnumerator>::operator*() const
	{
		return _currentProjectedValue;
	}
}
