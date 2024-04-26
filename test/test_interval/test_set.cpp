#include "gtest/gtest.h"
#include "set.h"
#include <set>

auto all_elements = std::set<std::string>{"mario", "luigi", "peach", "toad"};

TEST(Intersection, Set){
    auto set1 = SimpleSet("mario", all_elements);
    auto complement = set1.complement();

    auto result = Set(SimpleSetType<SimpleSet>{SimpleSet("luigi", all_elements),
                                            SimpleSet("peach", all_elements),
                                            SimpleSet("toad", all_elements)}, all_elements);
    EXPECT_EQ(complement, result);
    EXPECT_TRUE(set1.contains("mario"));
    EXPECT_FALSE(set1.contains("luigi"));
    EXPECT_TRUE(complement.contains("luigi"));
}

TEST(Emptyness, Set){
    auto set1 = SimpleSet(all_elements);
    EXPECT_TRUE(set1.is_empty());
}
