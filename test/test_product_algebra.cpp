#include <gtest/gtest.h>
#include "product_algebra.h"  // Include your SimpleEvent header file
#include "interval.h"  // Include your CPPInterval header file
#include "set.h"  // Include your CPPSetElement header file
#include "sigma_algebra.h"  // Include your CPPAbstractSimpleSet header file
#include "variable.h"
#include <memory>

// run with g++ -std=c++17 test_intersection.cpp product_algebra_cpp.cpp interval_cpp.cpp set_cpp.cpp sigma_algebra_cpp.cpp -lgtest -lgtest_main -pthread -o test_intersection

TEST(SimpleEventTest, IntersectionWith) {
    std::cout << "SimpleEventTest, IntersectionWith" << std::endl;
    // Define the sets and intervals
    auto sa = make_shared_set_element(1, make_shared_all_elements(4));
    auto sb = make_shared_set_element(2, make_shared_all_elements(4));
    auto sc = make_shared_set_element(3, make_shared_all_elements(4));
    std::cout << "set elements creation works" << std::endl;

    auto sabc = make_shared_simple_set_set();
    sabc->insert(sa);
    sabc->insert(sb);
    sabc->insert(sc);

    auto sab = make_shared_simple_set_set();
    sab->insert(sa);
    sab->insert(sb);

    auto a_a = make_shared_symbolic("a", make_shared_set(sa, make_shared_all_elements(4)));
    auto a_b = make_shared_symbolic("a", make_shared_set(sb, make_shared_all_elements(4)));
    auto a_ab = make_shared_symbolic("a", make_shared_set(sab, make_shared_all_elements(4)));
    auto a_abc = make_shared_symbolic("a", make_shared_set(sabc, make_shared_all_elements(4)));
    auto c = make_shared_symbolic("c", make_shared_set(sc, make_shared_all_elements(4)));
    auto x = make_shared_continuous("x");
    auto y = make_shared_continuous("y");

    auto var = make_shared_variable_set();
    var->insert(a_ab);
    var->insert(x);
    var->insert(y);

    auto var2 = make_shared_variable_set();
    var2->insert(a_a);
    var2->insert(x);

    auto var3 = make_shared_variable_set();
    var3->insert(c);

    auto var_exp = make_shared_variable_set();
    var_exp->insert(a_a);
    var_exp->insert(x);
    var_exp->insert(y);

    // Create SimpleEvent instances
    auto event1 = make_shared_simple_event(var);
    auto event2 = make_shared_simple_event(var2);
    auto event_exp = make_shared_simple_event(var_exp);

    // Perform intersection
    std::cout << "before first intersection " << std::endl;
    auto intersection = event1->intersection_with(event2);
    std::cout << "first intersection " << *intersection->to_string() << std::endl;

    // Assertions
    ASSERT_EQ(intersection, event_exp);
    ASSERT_NE(intersection, event1);

    // Test for empty intersection
    auto event3 = make_shared_simple_event(var3);
    std::cout << "before second intersection " << std::endl;
    auto second_intersection = std::static_pointer_cast<SimpleEvent>(event1->intersection_with(event3));
    std::cout << "second intersection " << *second_intersection->to_string() << std::endl;
    ASSERT_TRUE(second_intersection->is_empty());
}

//int main(int argc, char **argv) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}

//#include "variable.h"
//#include "gtest/gtest.h"
//#include "set.h"
//#include "product_algebra.h"
//
//
//auto x = make_shared_continuous(std::make_shared<std::string>("x"));
//auto y = make_shared_continuous(std::make_shared<std::string>("y"));
//
//
//auto a = make_shared_symbolic(std::make_shared<std::string>("a"),
//        make_shared_all_elements(std::set<std::string>{"a", "b", "c"}));
//auto u = make_shared_symbolic(std::make_shared<std::string>("u"),
//                              make_shared_all_elements(std::set<std::string>{"u", "v", "w"}));
//
//
//TEST(ProductAlgebra, VariableGetting) {
//    auto map1 = std::make_shared<VariableMap>();
//    map1->insert({x, x->domain});
//    map1->insert({y, y->domain});
//    map1->insert({y, y->domain});
//    auto event1 = SimpleEvent(map1);
//
//    auto map2 = std::make_shared<VariableMap>();
//    map2->insert({x, x->domain});
//    map2->insert({a, a->domain});
//    auto event2 = SimpleEvent(map2);
//
//    ASSERT_TRUE(map1->size() == 2);
//
//    auto event2_variables = event2.get_variables();
//    ASSERT_TRUE(event2_variables.size() == 2);
//    auto all_variables = event2.merge_variables(event1.get_variables());
//    ASSERT_TRUE(all_variables.size() == 3);
//}
//
//TEST(ProductAlgebra, Intersection) {
//    auto variables1 = VariableSet({x, y});
//    auto variables2 = VariableSet({y, a});
//    auto event1 = make_shared_simple_event(variables1);
//    auto event2 = make_shared_simple_event(variables2);
//    auto intersection = std::static_pointer_cast<SimpleEvent>(event1->intersection_with(event2));
//    ASSERT_TRUE(intersection->variable_map->size() == 3);
//    ASSERT_FALSE(intersection->is_empty());
//}
//
//TEST(ProductAlgebra, Complement){
//    auto variables = VariableSet({x, a});
//    auto event = make_shared_simple_event(variables);
//    auto complement = event->complement();
//    ASSERT_TRUE(complement->empty());
//    (*event->variable_map)[a] = make_shared_set(make_shared_set_element("a", a->domain->all_elements), a->domain->all_elements);
//    (*event->variable_map)[x] = closed(0, 1);
//    complement = event->complement();
//    ASSERT_EQ(complement->size(), 2);
//}
//
//TEST(ProductAlgebra, SimplifyOnce){
//    auto variables = VariableSet({x, y});
//    auto event1 = make_shared_simple_event(variables);
//    (*event1->variable_map)[x] = closed(0, 1);
//    (*event1->variable_map)[y] = closed(0, 1);
//
//    auto event2 = make_shared_simple_event(variables);
//    (*event2->variable_map)[x] = closed(0, 1);
//    (*event2->variable_map)[y] = closed(1, 2);
//    auto composite_event = make_shared_event(event1);
//    composite_event->simple_sets->insert(event2);
//    auto [result, changed] = composite_event->simplify_once();
//    ASSERT_TRUE(changed);
//    ASSERT_EQ(result->simple_sets->size(), 1);
//
//    auto contained_event = std::static_pointer_cast<SimpleEvent>(*result->simple_sets->begin());
//    ASSERT_EQ(contained_event->variable_map->size(), 2);
//    ASSERT_EQ(contained_event->variable_map->at(x)->simple_sets->size(), 1);
//    ASSERT_EQ(*contained_event->variable_map->at(y), *closed(0, 2));
//
//    auto simplified = composite_event->simplify();
//    ASSERT_EQ(*simplified, *result);
//
//}