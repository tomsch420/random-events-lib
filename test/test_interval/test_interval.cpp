#include "gtest/gtest.h"
#include "interval.h"


TEST(AtomicIntervalCreationTestSuite, SimpleInterval){
    SimpleInterval interval = *open(0.0, 1.0).simple_sets.begin();
    interval.lower = 0.0;
    interval.upper = 1.0;
    interval.left = BorderType::OPEN;
    interval.right = BorderType::CLOSED;
    EXPECT_EQ(interval.lower, 0.0);
    EXPECT_EQ(interval.upper, 1.0);
    EXPECT_EQ(interval.left, BorderType::OPEN);
    EXPECT_EQ(interval.right, BorderType::CLOSED);
}

TEST(AtomicIntervalIntersectionTestSuite, SimpleInterval){
    auto simple_interval_1 = SimpleInterval{0.0, 1.0, BorderType::OPEN, BorderType::CLOSED};
    auto simple_interval_2 = SimpleInterval{0.5, 1.5, BorderType::CLOSED, BorderType::OPEN};
    SimpleInterval intersection = simple_interval_1.intersection_with(simple_interval_2);
    EXPECT_EQ(intersection.lower, 0.5);
    EXPECT_EQ(intersection.upper, 1.0);
    EXPECT_EQ(intersection.left, BorderType::CLOSED);
    EXPECT_EQ(intersection.right, BorderType::CLOSED);
}

TEST(AtomicIntervalEmptyIntersectionTestSuite, SimpleInterval){
    auto interval1 = SimpleInterval{0.0, 1.0, BorderType::OPEN, BorderType::CLOSED};
    auto interval2 = SimpleInterval{2., 3., BorderType::CLOSED, BorderType::OPEN};
    SimpleInterval intersection = interval1.intersection_with(interval2);
    EXPECT_EQ(intersection.lower, 0.);
    EXPECT_EQ(intersection.upper, 0.);
    EXPECT_EQ(intersection.left, BorderType::OPEN);
    EXPECT_EQ(intersection.right, BorderType::OPEN);
}

TEST(AtomicIntervalContainsTestSuite, SimpleInterval){
    auto interval = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
    EXPECT_TRUE(interval.contains(0.5));
    EXPECT_TRUE(interval.contains(0.0));
    EXPECT_TRUE(interval.contains(1.0));
    EXPECT_FALSE(interval.contains(1.5));

    auto interval2 = SimpleInterval{0.0, 1.0, BorderType::OPEN, BorderType::CLOSED};
    EXPECT_TRUE(interval2.contains(0.5));
    EXPECT_FALSE(interval2.contains(0.0));
    EXPECT_TRUE(interval2.contains(1.0));
    EXPECT_FALSE(interval2.contains(1.5));
}

TEST(AtomicIntervalInvertTestSuite, SimpleInterval){
    auto interval = SimpleInterval{.0, 1.0, BorderType::OPEN, BorderType::CLOSED};
    Interval inverted = interval.complement();
    auto element_1 = SimpleInterval{-std::numeric_limits<float>::infinity(), 0.0,
                                    BorderType::OPEN, BorderType::CLOSED};
    auto element_2 = SimpleInterval{1.0, std::numeric_limits<float>::infinity(),
                                    BorderType::OPEN, BorderType::OPEN};
    auto inverted_by_hand = SimpleSetType<SimpleInterval>{element_1, element_2};
    EXPECT_EQ(inverted.simple_sets, inverted_by_hand);
}

TEST(AtomicIntervalIsEmptyTestSuite, SimpleInterval){
    auto interval = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
    EXPECT_FALSE(interval.is_empty());

    auto interval2 = SimpleInterval {0, 0, BorderType::CLOSED, BorderType::CLOSED};
    EXPECT_FALSE(interval2.is_empty());

    auto interval3 = SimpleInterval{1, 1, BorderType::OPEN, BorderType::OPEN};
    EXPECT_TRUE(interval3.is_empty());
}

TEST(AtomicIntervalDifferenceTest, SimpleInterval){
    auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
    auto interval2 = SimpleInterval {0., 1., BorderType::CLOSED, BorderType::CLOSED};
    auto interval3 = SimpleInterval {1., 2., BorderType::OPEN, BorderType::CLOSED};
    auto interval4 = SimpleInterval {0., 3., BorderType::OPEN, BorderType::CLOSED};

    Interval empty_difference = interval1.difference_with(interval2);
    EXPECT_TRUE(empty_difference.is_empty());

    Interval identity_difference = interval1.difference_with(interval3);
    EXPECT_EQ(identity_difference.simple_sets.size(), 1);
    EXPECT_EQ(identity_difference.simple_sets, SimpleSetType<SimpleInterval>{interval1});

    Interval difference_from_middle_element = interval4.difference_with(interval3);
    auto difference_from_middle_element_by_hand = SimpleSetType<SimpleInterval>{SimpleInterval(0., 1., BorderType::OPEN, BorderType::CLOSED),
                                                                                    SimpleInterval(2., 3., BorderType::OPEN, BorderType::CLOSED)};
    EXPECT_TRUE(difference_from_middle_element.is_disjoint());
    EXPECT_EQ(difference_from_middle_element.simple_sets, difference_from_middle_element_by_hand);
}

TEST(SimplifyIntervalTestSuite, Interval){
    auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::OPEN};
    auto interval2 = SimpleInterval{1.0, 1.5, BorderType::CLOSED, BorderType::OPEN};
    auto interval3 = SimpleInterval{1.5, 2.0, BorderType::OPEN, BorderType::CLOSED};
    auto interval4 = SimpleInterval{3.0, 5.0, BorderType::CLOSED, BorderType::CLOSED};

    auto interval = Interval{std::unordered_set<SimpleInterval>{interval1, interval2, interval3, interval4}};
    interval = interval.simplify();

    auto result_by_hand = Interval{std::unordered_set<SimpleInterval>{SimpleInterval{0.0, 1.5, BorderType::CLOSED, BorderType::OPEN},
                                                                      SimpleInterval{1.5, 2, BorderType::OPEN, BorderType::CLOSED},
                                                                      SimpleInterval{3.0, 5., BorderType::CLOSED, BorderType::CLOSED}}};
    EXPECT_EQ(interval, result_by_hand);

}

//
//TEST(IntervalIntervalDifferenceTest, Interval){
//    auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//    auto interval2 = SimpleInterval {0., 2., BorderType::CLOSED, BorderType::CLOSED};
//    auto interval3 = SimpleInterval {1., 4., BorderType::OPEN, BorderType::CLOSED};
//    Interval composed_interval_1 = Interval(std::vector<SimpleInterval>{interval2, interval3});
//    Interval composed_interval_2 = Interval(std::vector<SimpleInterval>{interval1, interval2});
//    auto difference = composed_interval_1.difference_with(composed_interval_2);
//    EXPECT_EQ(difference.simple_sets.size(), 1);
//}
//
//TEST(AtomicIntervalDifferenceWithIntervalTest, SimpleInterval){
//    auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//    auto interval2 = SimpleInterval {0., 2., BorderType::CLOSED, BorderType::CLOSED};
//    auto interval3 = SimpleInterval {1., 4., BorderType::OPEN, BorderType::CLOSED};
//    Interval shitty_interval = Interval(std::vector<SimpleInterval>{interval2, interval3});
//    auto difference = interval1.difference_with(shitty_interval);
//    EXPECT_TRUE(difference.is_empty());
//
//    Interval other_shitty_interval = Interval(std::vector<SimpleInterval>{interval1, interval2});
//    auto difference2 = interval3.difference_with(other_shitty_interval);
//    EXPECT_EQ(difference2.simple_sets.size(), 1);
//}
//

//
//TEST(IntervalMakeDisjointTestSuite, Interval){
//    auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//    auto interval2 = SimpleInterval{0.5, 1.5, BorderType::CLOSED, BorderType::CLOSED};
//    auto interval3 = SimpleInterval{1.5, 2.0, BorderType::CLOSED, BorderType::CLOSED};
//    auto interval4 = SimpleInterval{2.0, 3.0, BorderType::CLOSED, BorderType::CLOSED};
//    Interval composed_interval = Interval{std::vector<SimpleInterval>{interval1, interval2, interval3, interval4}};
//    Interval disjoint_interval = composed_interval.make_disjoint();
//    EXPECT_EQ(disjoint_interval.simple_sets.size(), 1);
//}
//
//TEST(UniqueCombinationsTestCase, SimpleInterval){
//    auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//    auto interval2 = SimpleInterval{0.5, 1.5, BorderType::CLOSED, BorderType::CLOSED};
//    auto interval3 = SimpleInterval{1.5, 2.0, BorderType::CLOSED, BorderType::CLOSED};
//    Interval composed_interval = Interval{std::vector<SimpleInterval>{interval1, interval2}};
//    auto combinations = unique_combinations(composed_interval.simple_sets);
//    EXPECT_EQ(combinations.size(), 1);
//
//    Interval composed_interval2 = Interval{std::vector<SimpleInterval>{interval1, interval2, interval3}};
//    auto combinations2 = unique_combinations(composed_interval2.simple_sets);
//    EXPECT_EQ(combinations2.size(), 3);
//}
//
//TEST(IntersectionWithAtomic, Interval){
//    auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//    auto interval2 = SimpleInterval{1.5, 2.5, BorderType::CLOSED, BorderType::CLOSED};
//    auto interval3 = SimpleInterval{0.5, 2.0, BorderType::CLOSED, BorderType::CLOSED};
//
//    auto interval = Interval{std::vector<SimpleInterval>{interval1, interval2}};
//    auto intersection = interval.intersection_with(interval3);
//    EXPECT_EQ(intersection.simple_sets.size(), 2);
//    EXPECT_TRUE(intersection.is_disjoint());
//    EXPECT_EQ(intersection.simple_sets[0].lower, 0.5);
//    EXPECT_EQ(intersection.simple_sets[0].upper, 1.0);
//
//    EXPECT_EQ(intersection.simple_sets[1].lower, 1.5);
//    EXPECT_EQ(intersection.simple_sets[1].upper, 2.0);
//}