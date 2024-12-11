#include "gtest/gtest.h"
#include "sigma_algebra.h"
#include "interval.h"
#include "variable.h"
#include "set.h"

TEST(Symbolic, ConstructorAndCompartor) {
    auto name = std::make_shared<std::string>("x");
    auto set_element = make_shared_set_element(0, make_shared_all_elements(3));
    auto symbol = make_shared_symbolic(name, make_shared_set(set_element, make_shared_all_elements(2)));
    EXPECT_EQ(symbol->name, name);
    EXPECT_EQ(symbol->domain->simple_sets->size(), 1);

    auto name2 = std::make_shared<std::string>("y");
    auto set_element2 = make_shared_set_element(0, make_shared_all_elements(2));
    auto symbol2 = make_shared_symbolic(name2, make_shared_set(set_element2, make_shared_all_elements(2)));
    EXPECT_NE(symbol.get(), symbol2.get());
    EXPECT_LT(*symbol, *symbol2);
}

TEST(Continuous, Constructor) {
    auto name = std::make_shared<std::string>("x");
    auto real = make_shared_continuous(name);
    auto reals_to_compare = reals();
    EXPECT_EQ(real->name.get()->compare("x"), 0);
    EXPECT_EQ(*reals_to_compare, *real.get()->domain.get());
}
