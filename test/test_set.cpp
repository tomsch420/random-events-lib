#include "gtest/gtest.h"
#include "set.h"
#include <set>

TEST(SetElement, IntType) {
    auto all_elements = make_shared_all_elements<int>(std::set{4, 3, 6});
    auto element = make_shared_set_element<int>(3, all_elements);
    EXPECT_EQ(element->element_index, 0);
    EXPECT_FALSE(element->is_empty());
    EXPECT_EQ(*element->non_empty_to_string(), "0");
}

TEST(SetElement, FloatType) {
    auto all_elements = make_shared_all_elements<float>(std::set{1.1f, 2.2f, 3.3f});
    auto element = make_shared_set_element<float>(2.2f, all_elements);
    EXPECT_EQ(element->element_index, 1);
    EXPECT_FALSE(element->is_empty());
    EXPECT_EQ(*element->non_empty_to_string(), "1");
}

TEST(SetElement, DoubleType) {
    auto all_elements = make_shared_all_elements<double>(std::set{1.1, 2.2, 3.3});
    auto element = make_shared_set_element<double>(2.2, all_elements);
    EXPECT_EQ(element->element_index, 1);
    EXPECT_FALSE(element->is_empty());
    EXPECT_EQ(*element->non_empty_to_string(), "1");
}

TEST(SetElement, StringType) {
    auto all_elements = make_shared_all_elements<std::string>(std::set<std::string>{"a", "b", "c"});
    auto element = make_shared_set_element<std::string>("b", all_elements);
    EXPECT_EQ(element->element_index, 1);
    EXPECT_FALSE(element->is_empty());
    EXPECT_EQ(*element->non_empty_to_string(), "1");
}

TEST(SetElement, Constructor) {
    const AllSetElementsPtr_t<std::string> all_elements = make_shared_all_elements<std::string>(std::set<std::string>{"a", "b", "c"});
    const SetElement<std::string> set_element("b", all_elements);
    EXPECT_EQ(set_element.element_index, 1);
    EXPECT_EQ(set_element.all_elements, all_elements);

    SetElement<std::string> set_element2("c", all_elements);
    EXPECT_EQ(set_element2.element_index, 2);
    EXPECT_EQ(set_element2.all_elements, all_elements);

    SetElement set_element3(all_elements);
    EXPECT_EQ(set_element3.element_index, -1);
    EXPECT_EQ(set_element3.all_elements, all_elements);
}

TEST(SetElement, IntersectionWith) {
    AllSetElementsPtr_t<int> all_elements = make_shared_all_elements<int>(std::set{0, 1, 2});
    auto set_element1 = make_shared_set_element<int>(1, all_elements);
    auto set_element2 = make_shared_set_element<int>(2, all_elements);
    auto set_element3 = make_shared_set_element<int>(1, all_elements);

    auto result = std::static_pointer_cast<SetElement<int>>(set_element1->intersection_with(set_element2));
    EXPECT_EQ(result->element_index, -1);
    EXPECT_TRUE(result->is_empty());

    result = std::static_pointer_cast<SetElement<int>>(set_element1->intersection_with(set_element3));
    EXPECT_EQ(result->element_index, 1);
    EXPECT_FALSE(result->is_empty());
}

TEST(SetElement, Complement) {
    AllSetElementsPtr_t<int> all_elements = make_shared_all_elements<int>(std::set{0, 1, 2});
    auto set_element1 = make_shared_set_element<int>(1, all_elements);
    auto set_element2 = make_shared_set_element<int>(2, all_elements);

    auto result = set_element1->complement();
    EXPECT_EQ(result->size(), 2);
    EXPECT_EQ(result->count(set_element1), 0);
    EXPECT_EQ(result->count(set_element2), 1);
}

TEST(Set, Simplify){
    AllSetElementsPtr_t<std::string> all_elements = make_shared_all_elements<std::string>(std::set<std::string>{"a", "b", "c"});
    auto set_element1 = make_shared_set_element<std::string>("b", all_elements);
    auto set_element2 = make_shared_set_element<std::string>("c", all_elements);
    auto sets = make_shared_simple_set_set();
    sets->insert(set_element1);
    auto set = make_shared_set<std::string>(sets, all_elements);
    auto result = set->simplify();
    EXPECT_EQ(result->simple_sets->size(), 1);
}

TEST(Set, MakeNewEmpty) {
    AllSetElementsPtr_t<std::string> all_elements = make_shared_all_elements<std::string>(std::set<std::string>{"a", "b", "c"});
    auto set_element1 = make_shared_set_element<std::string>("b", all_elements);
    auto set_element2 = make_shared_set_element<std::string>("c", all_elements);
    auto sets = make_shared_simple_set_set();
    sets->insert(set_element1);
    sets->insert(set_element2);
    auto set = make_shared_set<std::string>(sets, all_elements);
    auto result = set->make_new_empty();

}


TEST(Set, Constructor) {
    AllSetElementsPtr_t<std::string> all_elements = make_shared_all_elements<std::string>(std::set<std::string>{"a", "b", "c"});
    auto set_element1 = make_shared_set_element<std::string>("b", all_elements);
    auto set_element2 = make_shared_set_element<std::string>("c", all_elements);
    auto sets = make_shared_simple_set_set();
    sets->insert(set_element1);
    sets->insert(set_element2);
    auto set = make_shared_set<std::string>(sets, all_elements);
    //EXPECT_EQ(set->simple_sets, sets);
    //EXPECT_EQ(set->all_elements, all_elements);
    EXPECT_EQ(set->simple_sets->size(), 2);
    auto b = set->simplify();
    auto b2 = b->simplify();
    EXPECT_EQ(b2->simple_sets->size(), 2);
    auto a = set->complement();

    EXPECT_EQ(a->simple_sets->size(), 1);
}

TEST(Set, UnionWith){
    auto all_elements = make_shared_all_elements<std::string>(std::set<std::string>{"a", "b", "c"});
    auto a = std::static_pointer_cast<AbstractCompositeSet>(make_shared_set<std::string>(all_elements));
    auto element = make_shared_set_element<std::string>("a", all_elements);
    auto a_ = a->union_with(element);
    EXPECT_EQ(a_->simple_sets->size(), 1);
}