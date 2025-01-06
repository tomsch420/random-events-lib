#include <gtest/gtest.h>
#include "product_algebra.h"  // Include your SimpleEvent header file
#include "interval.h"  // Include your CPPInterval header file
#include "set.h"  // Include your CPPSetElement header file
#include "sigma_algebra.h"  // Include your CPPAbstractSimpleSet header file
#include "variable.h"
#include <memory>

auto sa = make_shared_set_element(0, make_shared_all_elements(3));
auto sb = make_shared_set_element(1, make_shared_all_elements(3));
auto sc = make_shared_set_element(2, make_shared_all_elements(3));

TEST(SimpleEventTest, IntersectionWith) {
    // Define the sets and intervals
    auto sabc = make_shared_simple_set_set();
    sabc->insert(sa);
    sabc->insert(sb);
    sabc->insert(sc);

    auto sab = make_shared_simple_set_set();
    sab->insert(sa);
    sab->insert(sb);

    auto a_a = make_shared_symbolic("a", make_shared_set(sa, make_shared_all_elements(3)));
    auto a_b = make_shared_symbolic("a", make_shared_set(sb, make_shared_all_elements(3)));
    auto a_ab = make_shared_symbolic("a", make_shared_set(sab, make_shared_all_elements(3)));
    auto a_abc = make_shared_symbolic("a", make_shared_set(sabc, make_shared_all_elements(3)));
    auto c = make_shared_symbolic("a", make_shared_set(sc, make_shared_all_elements(3)));
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
    auto intersection = event1->intersection_with(event2);

    auto var_set_test = make_shared_variable_set();
    var_set_test->insert(a_a);

    auto var_set_test2 = make_shared_variable_set();
    var_set_test2->insert(a_ab);

    // ASSERT_EQ(*var_set_test, *var_set_test2);

    // Assertions
    ASSERT_TRUE(*intersection == *event_exp);
    ASSERT_TRUE(*intersection != *event1);

    // Test for empty intersection
    auto event3 = make_shared_simple_event(var3);
    auto second_intersection = std::static_pointer_cast<SimpleEvent>(event1->intersection_with(event3));
    ASSERT_TRUE(second_intersection->is_empty());
}

TEST(ProductAlgebra, VariableGetting) {
    // Define the sets and intervals
    auto sabc = make_shared_simple_set_set();
    sabc->insert(sa);
    sabc->insert(sb);
    sabc->insert(sc);

    auto sab = make_shared_simple_set_set();
    sab->insert(sa);
    sab->insert(sb);

    auto a_a = make_shared_symbolic("a", make_shared_set(sa, make_shared_all_elements(3)));
    auto a_b = make_shared_symbolic("a", make_shared_set(sb, make_shared_all_elements(3)));
    auto a_ab = make_shared_symbolic("a", make_shared_set(sab, make_shared_all_elements(3)));
    auto a_abc = make_shared_symbolic("a", make_shared_set(sabc, make_shared_all_elements(3)));
    auto c = make_shared_symbolic("a", make_shared_set(sc, make_shared_all_elements(3)));
    auto b_a = make_shared_symbolic("b", make_shared_set(sa, make_shared_all_elements(3)));
    auto x = make_shared_continuous("x");
    auto y = make_shared_continuous("y");

    auto map1 = std::make_shared<VariableMap>();
    map1->insert({a_a, a_a->domain});
    map1->insert({b_a, b_a->domain});
    map1->insert({a_ab, a_ab->domain});

    auto event1 = SimpleEvent(map1);

    auto map2 = std::make_shared<VariableMap>();
    map2->insert({x, x->domain});
    map2->insert({a_abc, a_abc->domain});

    auto event2 = SimpleEvent(map2);


    // should be two due to a_a and a_ab being the same variable
    ASSERT_TRUE(map1->size() == 2);

    auto event2_variables = event2.get_variables();

    // x, a_abc
    ASSERT_TRUE(event2_variables->size() == 2);

    auto all_variables = event2.merge_variables(event1.get_variables());
    // a_a, b_a, x
    ASSERT_TRUE(all_variables->size() == 3);
}

TEST(ProductAlgebra, Complement){
     // Define the sets and intervals
    auto sab = make_shared_simple_set_set();
    sab->insert(sa);
    sab->insert(sb);

    auto a_a = make_shared_symbolic("a", make_shared_set(sa, make_shared_all_elements(3)));
    auto a_b = make_shared_symbolic("a", make_shared_set(sb, make_shared_all_elements(3)));
    auto a_ab = make_shared_symbolic("a", make_shared_set(sab, make_shared_all_elements(3)));
    auto c = make_shared_symbolic("a", make_shared_set(sc, make_shared_all_elements(3)));
    auto b_a = make_shared_symbolic("b", make_shared_set(sa, make_shared_all_elements(3)));
    auto x = make_shared_continuous("x");
    auto y = make_shared_continuous("y");


    auto interval1 = SimpleInterval<>::make_shared(0.0, 1.0, BorderType::CLOSED, BorderType::OPEN);
    auto simple_interval = make_shared_simple_set_set();
    simple_interval->insert(interval1);

    auto variables = std::make_shared<VariableMap>();
    variables->insert({a_ab, a_ab->domain});
    variables->insert({x, Interval<>::make_shared(simple_interval)});
    variables->insert({y, y->domain});

    auto event = make_shared_simple_event(variables);

    auto complement = event->complement();

    // ASSERT_EQ(complement->size(), 2);

    auto expected_1 = std::make_shared<VariableMap>();
    expected_1->insert({c, c->domain});
    expected_1->insert({x, x->domain});
    expected_1->insert({y, y->domain});

    auto expected_2 = std::make_shared<VariableMap>();
    expected_2->insert({a_ab, a_ab->domain});
    expected_2->insert({x, variables->at(x)->complement()});
    expected_2->insert({y, y->domain});

    auto complement_1 = make_shared_simple_event(expected_1);
    auto complement_2 = make_shared_simple_event(expected_2);

    auto simple_set_set = make_shared_simple_set_set();
    simple_set_set->insert(complement_1);
    simple_set_set->insert(complement_2);

    ASSERT_TRUE(compare_sets(complement, simple_set_set));
}
// def test_simplify(self):
//         event_1 = SimpleEvent(
//             {self.a: Set(TestEnum.A, TestEnum.B), self.x: SimpleInterval(0, 1), self.y: SimpleInterval(0, 1)})
//         event_2 = SimpleEvent(
//             {self.a: Set(TestEnum.C), self.x: SimpleInterval(0, 1), self.y: Interval(SimpleInterval(0, 1))})
//         event = Event(event_1, event_2)
//         simplified = event.simplify()
//         self.assertEqual(len(simplified.simple_sets), 1)
//
//         result = Event(SimpleEvent(
//             {self.a: self.a.domain, self.x: Interval(SimpleInterval(0, 1)), self.y: Interval(SimpleInterval(0, 1))}))
//         self.assertEqual(simplified, result)
TEST(ProductAlgebra, Simplify){
    // Define the sets and intervals
    auto sabc = make_shared_simple_set_set();
    sabc->insert(sa);
    sabc->insert(sb);
    sabc->insert(sc);

    auto sab = make_shared_simple_set_set();
    sab->insert(sa);
    sab->insert(sb);

    auto a_a = make_shared_symbolic("a", make_shared_set(sa, make_shared_all_elements(3)));
    auto a_b = make_shared_symbolic("a", make_shared_set(sb, make_shared_all_elements(3)));
    auto a_ab = make_shared_symbolic("a", make_shared_set(sab, make_shared_all_elements(3)));
    auto a_abc = make_shared_symbolic("a", make_shared_set(sabc, make_shared_all_elements(3)));
    auto c = make_shared_symbolic("a", make_shared_set(sc, make_shared_all_elements(3)));
    auto x = make_shared_continuous("x");
    auto y = make_shared_continuous("y");

    auto interval1 = SimpleInterval<>::make_shared(0.0, 1.0, BorderType::CLOSED, BorderType::OPEN);
    auto simple_interval = make_shared_simple_set_set();
    simple_interval->insert(interval1);

    auto variables = std::make_shared<VariableMap>();
    variables->insert({a_ab, a_ab->domain});
    variables->insert({x, Interval<>::make_shared(simple_interval)});
    variables->insert({y, Interval<>::make_shared(simple_interval)});

    auto variables2 = std::make_shared<VariableMap>();
    variables2->insert({c, c->domain});
    variables2->insert({x, Interval<>::make_shared(simple_interval)});
    variables2->insert({y, Interval<>::make_shared(simple_interval)});

    auto event1 = make_shared_simple_event(variables);
    // (*event1->variable_map)[x] = closed(0, 1);
    // (*event1->variable_map)[y] = closed(0, 1);
    auto event2 = make_shared_simple_event(variables2);


    auto composite_event = make_shared_event(event1);
    composite_event->simple_sets->insert(event2);
    auto result = composite_event->simplify();

    ASSERT_EQ(result->simple_sets->size(), 1);

    auto contained_event = std::static_pointer_cast<SimpleEvent>(*result->simple_sets->begin());
    ASSERT_EQ(contained_event->variable_map->size(), 3);
    ASSERT_EQ(contained_event->variable_map->at(x)->simple_sets->size(), 1);

    auto simplified = composite_event->simplify();
    ASSERT_EQ(*simplified, *result);

}