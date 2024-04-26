#include <vector>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "interval.h"
#include "sigma_algebra.h"


SimpleInterval SimpleInterval::simple_set_intersection_with(const SimpleInterval &other) const {

    // get the new lower and upper bounds
    float new_lower = std::max(lower, other.lower);
    float new_upper = std::min(upper, other.upper);

    // return the empty interval if the new lower bound is greater than the new upper bound
    if (new_lower > new_upper) {
        return SimpleInterval();
    }

    // initialize the new borders
    BorderType new_left;
    BorderType new_right;

    // if the lower bounds are equal, intersect the borders
    if (lower == other.lower) {
        new_left = intersect_borders(left, other.left);
    } else {
        // else take the border of the interval with the lower bound
        new_left = lower == new_lower ? left : other.left;
    }

    // if the upper bounds are equal, intersect the borders
    if (upper == other.upper) {

        new_right = intersect_borders(right, other.right);
    } else {
        // else take the border of the interval with the upper bound
        new_right = upper == new_upper ? right : other.right;
    }

    return SimpleInterval{new_lower, new_upper, new_left, new_right};
}

Interval SimpleInterval::simple_set_complement() const {
    const SimpleSetType<SimpleInterval> resulting_intervals = {
            SimpleInterval{-std::numeric_limits<float>::infinity(),
                           lower,
                           BorderType::OPEN,
                           invert_border(left)},
            SimpleInterval{upper,
                           std::numeric_limits<float>::infinity(),
                           invert_border(right),
                           BorderType::OPEN}};
    return Interval(resulting_intervals);
}

bool SimpleInterval::simple_set_is_empty() const {
    return lower == upper and (left == BorderType::OPEN or right == BorderType::OPEN);
}

bool SimpleInterval::operator==(const SimpleInterval &other) const {
    return lower == other.lower and upper == other.upper and left == other.left and right == other.right;
}

bool SimpleInterval::simple_set_contains(const float &element) const {
    SimpleInterval singleton = SimpleInterval{element, element, BorderType::CLOSED, BorderType::CLOSED};
    return contains(singleton);
}

SimpleInterval::SimpleInterval(float lower, float upper, BorderType left, BorderType right) : lower(lower),
                                                                                              upper(upper),
                                                                                              left(left),
                                                                                              right(right){
    if (lower > upper) { throw std::invalid_argument("Lower bound must be less than or equal to upper bound."); }
}

SimpleInterval::operator std::string() {
    return to_string();
}

std::string SimpleInterval::to_string() {
    if (is_empty()) {
        return "∅";
    }
    char left_representation = left == BorderType::OPEN ? '(' : '[';
    char right_representation = right == BorderType::OPEN ? ')' : ']';
    return std::string(
            left_representation + std::to_string(lower) + ", " + std::to_string(upper) + right_representation);
}

Interval Interval::composite_set_simplify() {
    std::vector<SimpleInterval> result;
    auto sorted = simple_sets_as_vector();

    std::sort(sorted.begin(), sorted.end(), by_lower_ascending());
    result.push_back(sorted[0]);

    for (auto current_simple_interval = sorted.begin() + 1; current_simple_interval != sorted.end(); ++current_simple_interval) {
        auto last_simple_interval = result.back();
        if (last_simple_interval.upper == current_simple_interval->lower &&
            !(last_simple_interval.right == BorderType::OPEN and current_simple_interval->left == BorderType::OPEN)) {
            result.pop_back();
            result.emplace_back(last_simple_interval.lower, current_simple_interval->upper, last_simple_interval.left, current_simple_interval->right);
        } else {
            result.push_back(*current_simple_interval);
        }
    }
    return Interval(SimpleSetType<SimpleInterval> (result.begin(), result.end()));
}


//size_t SimpleInterval::operator()(const SimpleInterval &interval) const {
//    return std::hash<float>()(interval.lower) ^ std::hash<float>()(interval.upper) ^ std::hash<int>()(
//            static_cast<int>(interval.left)) ^ std::hash<int>()(static_cast<int>(interval.right));
//}
//
//Interval Interval::make_disjoint() {
//    Interval disjoint;
//    Interval intersections;
//
//    Interval current_disjoint;
//
//    std::tie(disjoint, intersections) = split_into_disjoint_and_non_disjoint();
//
//    while (!intersections.is_empty()) {
//        std::tie(current_disjoint, intersections) = intersections.split_into_disjoint_and_non_disjoint();
//        extend_vector(disjoint.simple_sets, current_disjoint.simple_sets);
//    }
//
//    disjoint.simple_sets.erase(unique(disjoint.simple_sets.begin(), disjoint.simple_sets.end()),
//                               disjoint.simple_sets.end());
//
//    return disjoint.simplify();
//}
//
//
//Interval Interval::intersection_with(SimpleInterval &simple) {
//    std::vector<SimpleInterval> intersections;
//    for (auto interval: simple_sets) {
//        SimpleInterval intersection = interval.intersection_with(simple);
//        if (!intersection.is_empty()) {
//            intersections.push_back(intersection);
//        }
//    }
//    return Interval(intersections);
//}


//bool Interval::contains(SimpleInterval element) const {
//    for (auto interval: simple_sets) {
//        auto intersection = interval.intersection_with(element);
//        element = element.difference_with(intersection).simple_sets[0];
//        if (element.is_empty()) {
//            return true;
//        }
//    }
//    return false;
//}

//Interval Interval::intersection_with(const Interval &other) {
//    Interval result;
//    for (auto atomic_i: simple_sets) {
//        for (auto atomic_j: other.simple_sets) {
//            auto intersection = atomic_i.intersection_with(atomic_j);
//            if (!intersection.is_empty()) {
//                result.simple_sets.push_back(intersection);
//            }
//        }
//    }
//    return result;
//}
//
//Interval Interval::difference_with(const Interval &other) {
//    Interval result = empty();
//    for (auto atomic: simple_sets) {
//        auto difference = atomic.difference_with(other);
//        extend_vector(result.simple_sets, difference.simple_sets);
//    }
//    return result;
//}
//



//Interval SimpleInterval::difference_with(const Interval &other) const {
//    Interval result;
//
//    auto elementwise_differences = std::vector<Interval>{};
//
//    for (const auto atomic: other.simple_sets) {
//        auto difference = difference_with(atomic);
//        elementwise_differences.push_back(difference);
//    }
//
//    result.simple_sets.push_back(*this);
//    for (const auto &elementwise_difference: elementwise_differences) {
//        result = result.intersection_with(elementwise_difference);
//    }
//
//    return result;
//}
//