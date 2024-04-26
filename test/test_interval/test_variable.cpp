#include "gtest/gtest.h"
#include "interval.h"
#include "variable.h"

TEST(Variable, Continuous) {
    Continuous continuous("x");
    EXPECT_EQ(continuous.name, "x");
    EXPECT_EQ(continuous.domain, reals());
}