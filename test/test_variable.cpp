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

TEST(Variable, Comparison){
    auto variable1 = Symbolic("x", Set({"a", "b", "c"}));
    auto variable2 = Symbolic("y", Set({"a", "b", "c"}));
    auto variable3 = Continuous("x");
    EXPECT_TRUE(variable1 != variable2);
    EXPECT_TRUE(variable1 == variable3);
    EXPECT_TRUE(variable2 != variable3);
    EXPECT_TRUE(variable1 < variable2);
    EXPECT_TRUE(variable2 > variable3);
}