#pragma once 

#include "enumerator.h"

namespace DefQuery
{
	template <typename TSourceEnumerator, typename TFilter>
	class where_enumerator : public enumerator<typename TSourceEnumerator::value_type, where_enumerator<TSourceEnumerator, TFilter>>
	{
	public:
		where_enumerator(const TSourceEnumerator& source, const TFilter& filter);

		where_enumerator(const where_enumerator& other) = default;
		where_enumerator(where_enumerator&& other) = default;
		where_enumerator& operator=(const where_enumerator& other) = default;
		where_enumerator& operator=(where_enumerator&& other) = default;

		bool operator++();
		const typename TSourceEnumerator::value_type& operator*() const;

	private:
		bool move_next() override { return this->operator++(); }
		const typename TSourceEnumerator::value_type& current() const override { return this->operator*(); }

		TSourceEnumerator _source;
		TFilter _filter;
	};

	// ==============================================================================================

	template <typename TValue, typename TDerived>
	template <typename TFilter>
	where_enumerator<TDerived, TFilter> enumerator<TValue, TDerived>::where(const TFilter& filter)
	{
		return where_enumerator<TDerived, TFilter>(static_cast<TDerived&>(*this), filter);
	}

	template<typename TSourceEnumerator, typename TFilter>
	where_enumerator<TSourceEnumerator, TFilter>::where_enumerator(const TSourceEnumerator& source, const TFilter& filter)
		: _source(source)
		, _filter(filter)
	{
	}

	template<typename TSourceEnumerator, typename TFilter>
	bool where_enumerator<TSourceEnumerator, TFilter>::operator++()
	{
		while (++_source)
		{
			auto itemPassedTheFilter = _filter(*_source);
			if (itemPassedTheFilter)
				return true;
		}

		return false;
	}

	template<typename TSourceEnumerator, typename TFilter>
	const typename TSourceEnumerator::value_type& where_enumerator<TSourceEnumerator, TFilter>::operator*() const
	{
		return *_source;
	}
}
