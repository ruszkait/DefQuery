#include "gtest/gtest.h"
#include <list>
#include "../DefQuery/from.h"
#include "../DefQuery/orderby.h"

TEST(OrderByTest, MultiStageSortingTest)
{
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
        .orderby([](const auto& personA, const auto& personB) { return personA._name.compare(personB._name); })
        .thenby([](const auto& personA, const auto& personB) { return int(personA._age - personB._age); })
        .thenby([](const auto& personA, const auto& personB) { return personA._pets.size() < personB._pets.size() ? -1 : personA._pets.size() == personB._pets.size() ? 0 : +1; });
    
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Hanna", enumerator->_name);
    ASSERT_EQ(11, enumerator->_age);
    ASSERT_EQ(1, enumerator->_pets.size());
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Hanna", enumerator->_name);
    ASSERT_EQ(11, enumerator->_age);
    ASSERT_EQ(2, enumerator->_pets.size());
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Hanna", enumerator->_name);
    ASSERT_EQ(14, enumerator->_age);
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Hanna", enumerator->_name);
    ASSERT_EQ(19, enumerator->_age);
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Oliver", enumerator->_name);
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Peter", enumerator->_name);
    ASSERT_FALSE(++enumerator);
    ASSERT_FALSE(++enumerator);
}

TEST(OrderByTest, EmptyTest)
{
    struct Person
    {
        std::string _name;
        double _age;
        std::vector<std::string> _pets;
    };
    
    std::vector<Person> persons;
    
    auto enumerator = DefQuery::from(persons)
        .orderby([](const auto& personA, const auto& personB) { return personA._name.compare(personB._name); })
        .thenby([](const auto& personA, const auto& personB) { return int(personA._age - personB._age); });
    
    ASSERT_FALSE(++enumerator);
    ASSERT_FALSE(++enumerator);
}

TEST(OrderByTest, SmallWindowSizeTest)
{
    struct Person
    {
        std::string _name;
        double _age;
    };
    
    std::vector<Person> persons = {
        Person{ "Oliver", 10 },
        Person{ "Hanna", 19 },
        Person{ "Hanna", 11 },
        Person{ "Hanna", 11 },
        Person{ "Hanna", 14 },
        Person{ "Peter", 20 }
    };
    
    auto enumerator = DefQuery::from(persons)
        .orderby([](const auto& personA, const auto& personB) { return personA._name.compare(personB._name); })
        .thenby([](const auto& personA, const auto& personB) { return int(personA._age - personB._age); })
        .window_size(2);
    
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Hanna", enumerator->_name);
    ASSERT_EQ(19, enumerator->_age);
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Hanna", enumerator->_name);
    ASSERT_EQ(11, enumerator->_age);
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Hanna", enumerator->_name);
    ASSERT_EQ(11, enumerator->_age);
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Hanna", enumerator->_name);
    ASSERT_EQ(14, enumerator->_age);
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Oliver", enumerator->_name);
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Peter", enumerator->_name);
    ASSERT_FALSE(++enumerator);
    ASSERT_FALSE(++enumerator);
}

TEST(OrderByTest, DescendingOrderTest)
{
    struct Person
    {
        std::string _name;
        double _age;
    };
    
    std::vector<Person> persons = {
        Person{ "Hanna", 9 },
        Person{ "Oliver", 10 },
        Person{ "Hanna", 19 },
        Person{ "Peter", 20 }
    };
    
    
    auto enumerator = DefQuery::from(persons)
        .orderby([](const auto& personA, const auto& personB) { return personA._name.compare(personB._name); }, DefQuery::sorting_order::descending)
        .thenby([](const auto& personA, const auto& personB) { return int(personA._age - personB._age); }, DefQuery::sorting_order::ascending);
    
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Peter", enumerator->_name);
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Oliver", enumerator->_name);
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Hanna", enumerator->_name);
    ASSERT_EQ(9, enumerator->_age);
    ASSERT_TRUE(++enumerator);
    ASSERT_EQ("Hanna", enumerator->_name);
    ASSERT_EQ(19, enumerator->_age);
    ASSERT_FALSE(++enumerator);
    ASSERT_FALSE(++enumerator);
}
