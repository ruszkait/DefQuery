#pragma once

#include <type_traits>
#include <memory>
#include <list>
#include <vector>
#include <functional>


namespace DefQuery
{
	template <typename TSourceEnumerator, typename TFilter>
	class where_enumerator;

	template <typename TSourceEnumerator, typename TProjection>
	class select_enumerator;

	template <typename TSourceEnumerator, typename TEnumeratorProjection>
	class selectmany_enumerator;

	template <typename TSourceEnumerator>
	class stlrange_adapter;

	template <typename TValue>
	class shared_enumerator;

    template <typename TSourceEnumerator>
    class orderby_enumerator;
    enum class sorting_order
    {
        ascending,
        descending
    };

    template <typename TLeftEnumerator, typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
    class join_enumerator;
    
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
	};

	template <typename TValue, typename TDerived>
	class enumerator : public enumerator_interface<TValue>
	{
	public:
        using value_type = TValue;
        
		// Keeps the data that passes the filter predicate in the stream
		template <typename TFilter>
		where_enumerator<TDerived, TFilter> where(const TFilter& filter) &;
        template <typename TFilter>
        where_enumerator<TDerived, TFilter> where(const TFilter& filter) &&;

		// Transforms the source value to another type
		template <typename TProjection>
		select_enumerator<TDerived, TProjection> select(const TProjection& projector) &;
        template <typename TProjection>
        select_enumerator<TDerived, TProjection> select(const TProjection& projector) &&;

		// Flattens out a hierarchical container structure
		template <typename TEnumeratorProjection>
		selectmany_enumerator<TDerived, TEnumeratorProjection> selectmany(const TEnumeratorProjection& projector) &;
        template <typename TEnumeratorProjection>
        selectmany_enumerator<TDerived, TEnumeratorProjection> selectmany(const TEnumeratorProjection& projector) &&;

        // Erases the underlying decorator chain type.
        shared_enumerator<TValue> share() &;
        shared_enumerator<TValue> share() &&;

		// Creates an STL range from the enumerator
		stlrange_adapter<TDerived> stlrange() &;
        stlrange_adapter<TDerived> stlrange() &&;

		// Accumulates the values of the underlying enumerator to an accumlator
		template <typename TAccumlatorInitializer, typename TFolding>
		typename std::result_of<TAccumlatorInitializer(const TValue&)>::type aggregate(TFolding folder, TAccumlatorInitializer accumlatorInitializer);

		// Returns the number of elements
		std::size_t count();

        // Returns if any element fulfills the search criteria
        template <typename TFilter>
        bool contains(TFilter filter);
        bool contains(const TValue& searched);

        // Creates a list out of the query results
        std::list<TValue> list();

        // Creates a vector out of the query results
        std::vector<TValue> vector();

        // Sorting with a certain window size
        orderby_enumerator<TDerived> orderby(const std::function<int(const TValue&, const TValue&)>& comparator, sorting_order order = sorting_order::ascending) &;
        orderby_enumerator<TDerived> orderby(const std::function<int(const TValue&, const TValue&)>& comparator, sorting_order order = sorting_order::ascending) &&;

        // Returns the first element of the enumeration
        TValue first();
        
        // Joins two enumerators based on their keys
        template <typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
        join_enumerator<TDerived, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging> join(TRightEnumerator&& sourceRight, const TKeyProjectionLeft& keyProjectionLeft, const TKeyProjectionRight& keyProjectionRight, const TMerging& merging) &;
        template <typename TRightEnumerator, typename TKeyProjectionLeft, typename TKeyProjectionRight, typename TMerging>
        join_enumerator<TDerived, TRightEnumerator, TKeyProjectionLeft, TKeyProjectionRight, TMerging> join(TRightEnumerator&& sourceRight, const TKeyProjectionLeft& keyProjectionLeft, const TKeyProjectionRight& keyProjectionRight, const TMerging& merging) &&;

    protected:
        enumerator(bool exhausted)
            : _exhausted(exhausted)
        {}
        
        enumerator(const enumerator& other) = default;
        enumerator(enumerator&& other) = default;
        enumerator& operator=(const enumerator& other) = default;
        enumerator& operator=(enumerator&& other) = default;
        
        bool exhausted() const { return _exhausted; }
        bool is_valid() const { return !_exhausted; }
        void exhausted(bool value) { _exhausted = value; }
        
        bool _exhausted;
	};
}
