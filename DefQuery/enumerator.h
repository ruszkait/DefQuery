#pragma once

#include <type_traits>
#include <memory>

namespace DefQuery
{
	template <typename TSourceEnumerator, typename TFilter>
	class where_enumerator;

	template <typename TSourceEnumerator, typename TProjection, typename TProjectedValue>
	class select_enumerator;

	template <typename TSourceEnumerator, typename TEnumeratorProjection, typename TProjectedEnumeratorValue>
	class selectmany_enumerator;

	template <typename TSourceEnumerator>
	class stlrange_adapter;

	template <typename TValue>
	class shared_enumerator;


	template <typename TValue>
	class enumerator_interface
	{
	public:
		virtual ~enumerator_interface() = default;

		// Produces the next element, returns false if the enumeration has exhausted
        // Prefer the usage of the enumerator operator++() over this function,
        // this saves the cost of calling a virtual function
		virtual bool move_next() = 0;

		// Provides the last produced element
        // Prefer the usage of the enumerator operator*() over this function,
        // this saves the cost of calling a virtual function
        virtual const TValue& current() const = 0;

	protected:
		virtual std::shared_ptr<enumerator_interface<TValue>> clone() const = 0;
	};

	template <typename TValue, typename TDerived>
	class enumerator : public enumerator_interface<TValue>
	{
	public:
        using value_type = TValue;
        
		// Keeps the data that passes the filter predicate in the stream
		template <typename TFilter>
		where_enumerator<TDerived, TFilter> where(const TFilter& filter);

		// Transforms the source value to another type
		template <typename TProjection, typename TProjectedValue = typename std::result_of<TProjection(const TValue&)>::type>
		select_enumerator<TDerived, TProjection, TProjectedValue> select(const TProjection& projector);

		// Flattens out a hierarchical container structure
		template <typename TEnumeratorProjection, typename TProjectedEnumerator = typename std::result_of<TEnumeratorProjection(const TValue&)>::type >
		selectmany_enumerator<TDerived, TEnumeratorProjection, TProjectedEnumerator> selectmany(const TEnumeratorProjection& projector);

        // Erases the underlying decorator chain type.
		// Creates a shared pointer wrapper. This wrapper can be passed around by value cheap.
		// Then the shared enumerator can be further decorated by other enumerators.
        shared_enumerator<TValue> share();

		// Creates an STL range from the enumerator
		stlrange_adapter<TDerived> stlrange();

		// Accumulates the values of the underlying enumerator to an accumlator
		template <typename TAccumlatorInitializer, typename TFolding, typename TAccumlator = typename std::result_of<TAccumlatorInitializer(const TValue&)>::type>
		TAccumlator aggregate(TFolding folder, TAccumlatorInitializer accumlatorInitializer);

		// Returns the number of elements
		std::size_t count();

        // Returns the number of elements
        template <typename TFilter>
        bool contains(TFilter filter);

	protected:
		std::shared_ptr<enumerator_interface<TValue>> clone() const override;
	};

	template <typename TValue, typename TDerived>
	std::shared_ptr<enumerator_interface<TValue>> enumerator<TValue, TDerived>::clone() const
	{
		// Use make_shared to minimize the number of heap allocations
		return std::make_shared<TDerived>(static_cast<const TDerived&>(*this));
	}
}
