# DefQuery
Yet another C# LINQ implementation in C++.

## Features:
* Simple syntax: fluent interface with lambdas
* The enumerator chain is kept on the stack, heap allocations are minimized
* Enumerator chain can be copied or moved
* Needs at least C++14

## Starting an enumerator chain
You can use the **from** adapter that takes a container (or anything that has a begin() and end() method()) and start an enumerator chain. It is recommended to use the automatic type inference (**auto** keyword), to save yourself the headache to write the type of the enumerator. It will pay off even more once you have longer enumerator chains.
```
std::list<int> list = { 1,2,3 };
auto enumerator = DefQuery::from(list);
```
And now let us consume the enumerator. Unlike iterators, the enumerator know when it is exhausted, so every step forward delivers a boolean value telling if the stepping was successful. The * operator is used to get the current referenced value from the enumerator. 
```
while(++enumerator)
    std::cout << *enumerator;
```
If you want to use as a data source only a range of a container, or a range with pointers or just an integer range, use the **range** adapter.
```
std::vector<int> numbers = { 1,2,3 };
// STL range
auto containerRangeEnumerator = DefQuery::range(numbers.begin(), numbers.end());
// Pointer range
auto pointerRangeEnumerator = DefQuery::range(&numbers[0], &numbers[3]);
// Integer range
auto integerRangeEnumerator = DefQuery::range(0, 3);
```
If you want to generate an inginite sequence, you may use an generator with a stateful lambda.
```
auto quadraticNumbers = DefQuery::generator<int>([_currentIndependent = 0](int& nextValue) mutable
{
	// Calculate current value
	nextValue = _currentIndependent * _currentIndependent;
	// Move to next state
	++_currentIndependent;
	// Infinite sequence, never gets exhausted
	return true;
});
```

## Convert enumeration into STL range
Once you have an enumeration chain, you may want to convert it to an STL range. That range you may use in a range-based for loop or in STL algorithms.
```
std::vector<int> vector = { 1,2,3,4 };
auto enumerator = DefQuery::from(vector);

std::list<int> copiedValues;
auto stlRange = enumerator.stlrange();
```
Use a range based for look
```
for (auto number : stlRange)
	copiedValues.push_back(number);
```
 or an STL algorithm
```
std::copy(stlRange.begin(), stlRange.end(), std::back_inserter(copiedValues));
```

## Filtering
With filtering we can use a boolean function (predicate) that tells if the current value of the enumeration may go further in the chain or it gets discarded. Note that you use the fluent API (next stage is added with the dot operator) to join further elements to the end of the enumerator chain (this case the chain starts with a from adapter and continues with a filtering stage.
```
std::list<int> numbers = { 1,2,3,4,5,6 };
auto enumerator = DefQuery::from(numbers)
	.where([](const int currentNumber) { return currentNumber < 4 || currentNumber > 5; });
```
## Projection
We may need to change the type of the value of the enumerator chain. In the following example the container provides a list of Person objects. We are only interested in the age property, so we change the type from **Person** to **double**. In the **select** enumerator you have to provide the projection function. 
```
struct Person
{
	std::string _name;
	double _age;
};

std::vector<Person> siblings = { Person{ "Oliver", 10 }, Person{ "Hanna", 11 }, Person{ "Peter", 20 } };
auto siblingsAgeEnumerator = DefQuery::from(siblings)
	.select([](const Person& currentSibling) { return currentSibling._age; });
```
## Sorting
The enumerator chain sorting needs a comparator function to be able to decide which element comes after the other. The comparator shall be specified in a form of a lambda expression. When two elements evaluates to be equal by the comparator then further rules can be specified (**thenby(....)**) to decide about the order.
Sorting is a special enumeration case; all elements must be available to do the sorting correctly. So the sorting enumerator reads in all elements (when its operator++ is called the first time) from the previous stages, buffers them and sorts them. Then this sorted buffer can be consumed by the further stages.
```
struct Person
{
	std::string _name;
	double _age;
	std::vector<std::string> _pets;
};

std::vector<Person> persons = {
   Person{ "Oliver", 10 , { "dog", "cat" }},
   Person{ "Hanna", 19, { "fish", "bird" } },
   Person{ "Hanna", 11, { "fish", "bird" } },
   Person{ "Hanna", 11, { "fish" } },
   Person{ "Hanna", 14, { "fish", "bird" } },
   Person{ "Peter", 20, { "dog", "bird" } }
};

auto enumerator = DefQuery::from(persons)
	.orderby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._name.compare(personB._name); })
	.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return int(personA._age - personB._age); })
	.thenby(DefQuery::sorting_order::ascending, [](const auto& personA, const auto& personB) { return personA._pets.size() < personB._pets.size() ? -1 : personA._pets.size() == personB._pets.size() ? 0 : +1; });
```

## Collapse to container
Sometimes you may only want to put the result of the enumeration directly into a container. For these cases there are convinience functions.
```
std::array<int,4> numbers = { 1,2,3,4 };
auto myList = DefQuery::from(numbers)
	.list();
auto myVector = DefQuery::from(numbers)
	.vector();
// For complexer containers (like associative containers), there is no direct function, use the store()
auto squareMapping = DefQuery::from(numbers)
	.store<std::map<double, double>>([](auto& map, auto& number) { map.emplace(number, numer*number); });
```

## Type decaying
When the enumerator chain is built up, it creates many composed types. So we would get some composed templates like that: **select_enumerator<where_enumerator<from_enumerato<....>>>**. As long as we use the **auto** keyword we can hide this complexity.

But what can you do if you want to use such an enumeration in a function argument, or you want to return an enumeration and you do not want to use templates there (or auto as return type). Then you have to decay the type of the composed enumerator.

```
struct Person
{
	std::string _name;
	double _age;
};

std::vector<Person> siblings = { Person{ "Oliver", 10 }, Person{ "Hanna", 11 }, Person{ "Peter", 20 } };
decayed_enumerator<double> adultSiblingsAgeEnumerator = DefQuery::from(siblings)
	.where([](const Person& currentSibling) { return currentSibling._age > 18; })
	.select([](const Person& currentSibling) { return currentSibling._age; })
	.decay();
	
double calculateAverage(decayed_enumerator<double> enumeratorByValue);
```
Note that the composed chain has disappeared (we do not use **auto**) and the decayed_enumerator only depends on the enumeration value type. This comes at a cost: internally the decay moves the previous stages from the stack to the heap, so the chain is not only on the stack any more.

## Joining enumerator chains
Joining enumerators works the same as the inner join in SQL. For both sources a key element is specified. Once the key element matches in both sources, it creates a new entry in the output.
```
std::map<int, std::string> names = { {5, "John"}, {7, "James"}, {11, "Peter"} };
std::map<int, double> heights = { {7, 1.90}, {5, 1.80}, {11, 1.70} };

struct PersonData	{
    std::string _name;
	double _height;
};

auto enumerator = DefQuery::from(names)
	.join(DefQuery::from(heights),
	    // The key from the left side 
		[](const auto& namesEntry) { auto key = namesEntry.first; return key; },
	    // The key from the right side 
		[](const auto& heightsEntry) { auto key = heightsEntry.first; return key; },
		// Generator for the joined entries
		[](const auto& namesEntry, const auto& heightsEntry) { auto& name = namesEntry.second; auto height = heightsEntry.second; return PersonData{ name, height }; });
```

## Grouping
Turns the sequence of elements into a sequence of groups. Each group has a key and a vector of members.
```
struct Person {
	std::string _name;
	double _age;
	std::vector<std::string> _pets;
};

std::vector<Person> persons = {
   Person{ "Oliver", 10 , { "dog", "cat" }},
   Person{ "Hanna", 19, { "fish", "bird" } },
   Person{ "Arnold", 11, { "fish", "bird" } },
   Person{ "Henrik", 19, { "fish", "bird" } },
   Person{ "Peter", 20, { "dog", "bird" } },
   Person{ "Harald", 19, { "fish", "bird" } },
   Person{ "Agnes", 15, { "cat", "bird" } } 
};

// Group by the first letter of the name
auto enumerator = DefQuery::from(persons)
	.groupby([](const auto& person) { return person._name[0]; });	

while(++enumerator) {
    auto firstLetter = enumerator->_key;
        std::cout << "Names starting with letter " << firstLetter;
    for (const auto& groupMember : enumerator->_members)
        std::cout << groupMember._name;
}
```

## Further functionalities
* **all(<predicate>)**: returns if all elements of the chain satisfies the given condition
* **any(<predicate>)**: returns if at least one element of the chain satisfies the given condition
* **contains(<value> or <predicate>)**: similar to **any**, but can take a value as a parameter
* **first()**: returns the very first element of the enumeration, or throws an exception if there is none
* **reverse()**: reveres the order of the elements of an enumeration. Internally it stores every element in the chain in a buffer, so it may use a big amount of memeory.
* **take(<# elements to take>)**, **takewhile(<predicate>)**: takes the first N element of the enumeration or until the predicate is true.

## Deferred execution
When a chain is set up, the query is not executed yet. As soon as the chain is consumed, it produces only the next value. This makes it possible enumerate an enumeration chain that has an infinite source (like the sequence of the quadratic numbers). On the other hand it also minimizes the effort by focusing only on the creation of the next value.
This style of usage also eliminates the need to evalute the query, write the results into a container, and then pass this container to the next stage of processing.

## Lifecycle of a chain
* Enumerators can be consumed only once, and then they are "exhausted". There is no way to "rewind" the enumerator to the beginning.

* Normally enumerator chains are created on the stack. They do not own the underlying container, so it is up to the user to make sure that the container is still alive when the enumeration runs. Moreover the user must take care that the iterators used in the enumerator chain are not invalidated when the container changes. It is the safest not to change the container when it is being enumerated.

* When an iterator chain is copied then the whole chain gets cloned, including all state information of the chain. It means that consuming the orginal and the copied enumerator are completely independent.
```
std::list<int> list = { 1,6,3,8,9,2,7,5,4 };

// Adding extra stages, moves the previous stages to the new end of the chain.
auto originalEnumerator = DefQuery::from(list)
	.where([](const int a) { return a > 3 && a < 9; })
	.orderby(DefQuery::sorting_order::ascending);

// Because the "originalEnumerator" is a rvalue, the whole chain gets copied 
// and then the copy gets extended with new stages.
auto extendedEnumerator = originalEnumerator
	.where([](const int a) { return a % 2 == 0; })
	.select([](const int a) { return a * 10; });

// However the "extendedEnumerator" was spawned from the "originalEnumerator",
// they are completely independent, and iterated independently.
```
This feature makes the C# IEnumerable unnecessary here: if you want to go through the enumeration many times, just create the enumeration, use this first instance as a "master" instance. Create copies from the "master" instance and use them to iterate through

 * In case of moving the iterator chain, the state is moved to the new instance, and the original enumerator goes to exhausted state, so if it is consumed, it immediately ends. 
 ```
std::list<int> list = { 1,6,3,8,9,2,7,5,4 };

auto originalEnumerator = DefQuery::from(list)
	.where([](const int a) { return a > 3 && a < 9; })
	.orderby(DefQuery::sorting_order::ascending);

// Because the "originalEnumerator" is a lvalue, the whole chain gets moved into the "extendedEnumerator"
// and then it gets extended with new stages.
auto extendedEnumerator = std::move(originalEnumerator)
	.where([](const int a) { return a % 2 == 0; })
	.select([](const int a) { return a * 10; });

// The "extendedEnumerator" was can be consumed now, but the "originalEnumerator"
// is exhausted.
```
