#include "gtest/gtest.h"
#include "interval.h"
#include "sigma_algebra.h"
#include <set>


TEST(AtomicIntervalCreationTestSuite, SimpleInterval) {
    SimpleInterval interval = SimpleInterval();
    interval.lower = 0.0;
    interval.upper = 1.0;
    interval.left = BorderType::OPEN;
    interval.right = BorderType::CLOSED;
    EXPECT_EQ(interval.lower, 0.0);
    EXPECT_EQ(interval.upper, 1.0);
    EXPECT_EQ(interval.left, BorderType::OPEN);
    EXPECT_EQ(interval.right, BorderType::CLOSED);
}

TEST(AtomicIntervalIntersectionTestSuite, SimpleInterval) {
    auto simple_interval_1 = SimpleInterval{0.0, 1.0, BorderType::OPEN, BorderType::CLOSED};
    auto simple_interval_2 = SimpleInterval{0.5, 1.5, BorderType::CLOSED, BorderType::OPEN};
    auto intersection = *simple_interval_1.intersection_with(&simple_interval_2);

    auto intersection_by_hand = SimpleInterval{0.5, 1, BorderType::CLOSED, BorderType::CLOSED};
    EXPECT_EQ(intersection, intersection_by_hand);
}

TEST(AtomicIntervalEmptyIntersectionTestSuite, SimpleInterval) {
    auto interval1 = SimpleInterval{0.0, 1.0, BorderType::OPEN, BorderType::CLOSED};
    auto interval2 = SimpleInterval{2., 3., BorderType::CLOSED, BorderType::OPEN};
    SimpleInterval intersection = *interval1.intersection_with(&interval2);
    auto intersection_by_hand = SimpleInterval{0., 0, BorderType::OPEN, BorderType::OPEN};
    EXPECT_EQ(intersection, intersection_by_hand);;
    EXPECT_TRUE(intersection.is_empty());
}

// TEST(AtomicIntervalContainsTestSuite, SimpleInterval){
//     auto interval = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//     EXPECT_TRUE(interval.contains(0.5));
//     EXPECT_TRUE(interval.contains(0.0));
//     EXPECT_TRUE(interval.contains(1.0));
//     EXPECT_FALSE(interval.contains(1.5));
//
//     auto interval2 = SimpleInterval{0.0, 1.0, BorderType::OPEN, BorderType::CLOSED};
//     EXPECT_TRUE(interval2.contains(0.5));
//     EXPECT_FALSE(interval2.contains(0.0));
//     EXPECT_TRUE(interval2.contains(1.0));
//     EXPECT_FALSE(interval2.contains(1.5));
// }
//
TEST(AtomicIntervalInvertTestSuite, SimpleInterval) {
    auto interval = SimpleInterval{.0, 1.0, BorderType::OPEN, BorderType::CLOSED};
    auto inverted = *interval.complement();
    auto element_1 = SimpleInterval{-std::numeric_limits<float>::infinity(), 0.0,
                                    BorderType::OPEN, BorderType::CLOSED};
    auto element_2 = SimpleInterval{1.0, std::numeric_limits<float>::infinity(),
                                    BorderType::OPEN, BorderType::OPEN};
    auto element_3 = SimpleInterval{1.0, std::numeric_limits<float>::infinity(),
                                    BorderType::OPEN, BorderType::OPEN};
    auto inverted_by_hand = SimpleSetSet_t();
    inverted_by_hand.insert(&element_1);
    inverted_by_hand.insert(&element_2);
    inverted_by_hand.insert(&element_3);

    auto it = inverted.begin();
    auto it_by_hand = inverted_by_hand.begin();

    while (it != inverted.end()) {
        EXPECT_EQ(**it, **it_by_hand);
        it++;
        it_by_hand++;
    }

    auto boop = std::equal(inverted.begin(), inverted.end(), inverted_by_hand.begin());
    EXPECT_EQ(inverted_by_hand, inverted);

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

     auto empty_difference = interval1.difference_with(&interval2);
     EXPECT_TRUE(empty_difference->empty());

     auto identity_difference = interval1.difference_with(&interval3);
     EXPECT_EQ(identity_difference->size(), 1);
     EXPECT_EQ(**identity_difference->begin(), interval1);

     auto difference_from_middle_element = interval4.difference_with(&interval3);
     auto difference_from_middle_element_by_hand = SimpleSetSet_t{new SimpleInterval(0., 1., BorderType::OPEN, BorderType::CLOSED),
                                                                                     new SimpleInterval(2., 3., BorderType::OPEN, BorderType::CLOSED)};

    //EXPECT_EQ(difference_from_middle_element, difference_from_middle_element_by_hand);
 }

// TEST(SimplifyIntervalTestSuite, Interval){
//     auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::OPEN};
//     auto interval2 = SimpleInterval{1.0, 1.5, BorderType::CLOSED, BorderType::OPEN};
//     auto interval3 = SimpleInterval{1.5, 2.0, BorderType::OPEN, BorderType::CLOSED};
//     auto interval4 = SimpleInterval{3.0, 5.0, BorderType::CLOSED, BorderType::CLOSED};
//
//     auto interval = Interval{std::set<SimpleInterval>{interval1, interval2, interval3, interval4}};
//     interval = interval.simplify();
//
//     auto result_by_hand = Interval{std::set<SimpleInterval>{SimpleInterval{0.0, 1.5, BorderType::CLOSED, BorderType::OPEN},
//                                                                       SimpleInterval{1.5, 2, BorderType::OPEN, BorderType::CLOSED},
//                                                                       SimpleInterval{3.0, 5., BorderType::CLOSED, BorderType::CLOSED}}};
//     EXPECT_EQ(interval, result_by_hand);
//
// }
//
// TEST(SplitIntervalTestSuit, Interval){
//     auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::OPEN};
//     auto interval2 = SimpleInterval{1.0, 1.5, BorderType::CLOSED, BorderType::OPEN};
//     auto interval3 = SimpleInterval{1.5, 2.0, BorderType::OPEN, BorderType::CLOSED};
//     auto interval4 = SimpleInterval{3.0, 5.0, BorderType::CLOSED, BorderType::CLOSED};
//
//     auto interval = Interval{std::set<SimpleInterval>{interval1, interval2, interval3, interval4}};
//     auto [disjoint, non_disjoint] = interval.split_into_disjoint_and_non_disjoint();
//
//     EXPECT_TRUE(disjoint.is_disjoint());
// }
//
// TEST(IntervalMakeDisjointTestSuite, Interval){
//     auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval2 = SimpleInterval{0.5, 1.5, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval3 = SimpleInterval{1.5, 2.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval4 = SimpleInterval{2.0, 3.0, BorderType::CLOSED, BorderType::CLOSED};
//     Interval composed_interval = Interval{SimpleSetType<SimpleInterval>{interval1, interval2, interval3, interval4}};
//     Interval disjoint_interval = composed_interval.make_disjoint();
//     EXPECT_EQ(disjoint_interval.simple_sets.size(), 1);
//     EXPECT_TRUE(disjoint_interval.is_disjoint());
// }
//
// TEST(IntervalIntersectionSimple, Interval){
//     auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval2 = SimpleInterval{2.0, 3.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval3 = SimpleInterval{0.5, 2.5, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval = Interval{SimpleSetType<SimpleInterval>{interval1, interval2}};
//     auto intersection = interval.intersection_with(interval3);
//     EXPECT_TRUE(intersection.is_disjoint());
//     EXPECT_EQ(intersection.simple_sets.size(), 2);
// }
//
// TEST(IntervalIntersectionInterval, Interval){
//     auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval2 = SimpleInterval{2.0, 3.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval3 = SimpleInterval{4, 5, BorderType::CLOSED, BorderType::CLOSED};
//     auto composite_interval1 = Interval{SimpleSetType<SimpleInterval>{interval1, interval2}};
//     auto composite_interval2 = Interval{SimpleSetType<SimpleInterval>{interval2, interval3}};
//     auto intersection = composite_interval1.intersection_with(composite_interval2);
//     EXPECT_TRUE(intersection.is_disjoint());
//     EXPECT_EQ(intersection.simple_sets.size(), 1);
// }
//
// TEST(IntervalComplement, Interval){
//     auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval2 = SimpleInterval{2.0, 3.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval = Interval{SimpleSetType<SimpleInterval>{interval1, interval2}};
//     auto complement = interval.complement();
//     EXPECT_EQ(complement.simple_sets.size(), 3);
//     EXPECT_TRUE(complement.is_disjoint());
//     EXPECT_TRUE(complement.intersection_with(interval).is_empty());
// }
//
// TEST(IntervalUnion, Interval){
//     auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval2 = SimpleInterval{0.5, 1.5, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval3 = SimpleInterval{2.0, 3.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval = Interval{SimpleSetType<SimpleInterval>{interval1}};
//     auto other_interval = Interval{SimpleSetType<SimpleInterval>{interval2, interval3}};
//     auto union_ = interval.union_with(interval2);
//     EXPECT_EQ(union_.simple_sets.size(), 1);
//     EXPECT_TRUE(union_.is_disjoint());
//
//     auto union_2 = union_.union_with(other_interval);
//     EXPECT_EQ(union_2.simple_sets.size(), 2);
//     EXPECT_TRUE(union_2.is_disjoint());
// }
//
// TEST(IntervalDifference, Interval){
//     auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval2 = SimpleInterval{0.5, 1.5, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval3 = SimpleInterval{2.0, 3.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval = Interval{SimpleSetType<SimpleInterval>{interval1}};
//     auto other_interval = Interval{SimpleSetType<SimpleInterval>{interval2, interval3}};
//     auto difference = other_interval.difference_with(interval1);
//     EXPECT_EQ(difference.simple_sets.size(), 2);
//     EXPECT_TRUE(difference.is_disjoint());
//
//     auto difference_2 = other_interval.difference_with(difference);
//     EXPECT_EQ(difference_2.simple_sets.size(), 1);
//     EXPECT_TRUE(difference_2.is_disjoint());
// }
//
//
// TEST(IntervalContainment, Interval){
//     auto interval1 = SimpleInterval{0.0, 1.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval2 = SimpleInterval{2.0, 3.0, BorderType::CLOSED, BorderType::CLOSED};
//     auto interval = Interval{SimpleSetType<SimpleInterval>{interval1, interval2}};
//     EXPECT_TRUE(interval.contains(0.5));
//     EXPECT_FALSE(interval.contains(1.5));
//
//     auto interval3 = closed(2.5, 2.7);
//     EXPECT_TRUE(interval.contains(interval3));
//
//     auto interval4 = closed(0.5, 1.5);
//     EXPECT_FALSE(interval.contains(interval4));
//
//     EXPECT_TRUE(interval.contains(interval));
//
//
// }
