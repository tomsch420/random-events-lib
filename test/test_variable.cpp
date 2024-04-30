#include "gtest/gtest.h"
#include "interval.h"
#include "variable.h"
#include "set.h"

TEST(Variable, Continuous) {
    Continuous continuous("x");
    EXPECT_EQ(continuous.name, "x");
    EXPECT_EQ(continuous.domain, reals());
}

TEST(Variable, Symbolic) {
    auto variable = Symbolic("x", Set({"a", "b", "c"}));
    EXPECT_EQ(variable.name, "x");
    EXPECT_EQ(variable.domain, Set({"a", "b", "c"}));
}

TEST(Variable, Integer) {
    auto variable = Integer("x");
    EXPECT_EQ(variable.name, "x");
    EXPECT_EQ(variable.domain, reals());
}