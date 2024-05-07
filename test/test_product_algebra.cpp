#include "variable.h"
#include "gtest/gtest.h"
#include "set.h"
#include "product_algebra.h"


auto x = make_shared_continuous(std::make_shared<std::string>("x"));
auto y = make_shared_continuous(std::make_shared<std::string>("x"));

//auto all_elements_a = std::make_shared<AllSetElements>();
//all_elements_a->all_elements_set = {"a", "b", "c"};
//
// auto a = make_shared_symbolic("a", Set({"a", "b", "c"}));
// auto u = make_shared_symbolic("u", make({"u", "v", "w"}));
////
//// TEST(ProductAlgebra, Intersection){
////     auto unit_interval = closed(0, 1);
////     std::map<VariableVariant, SetVariant> vmap_1 = {{x, unit_interval}, {y, unit_interval}};
////     auto event1 = SimpleEvent(vmap_1);
////
//// }
