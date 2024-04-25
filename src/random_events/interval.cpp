#include <vector>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <iostream>
#include "interval.h"


SimpleInterval SimpleInterval::intersection_with(const SimpleInterval &other) const {

    // get the new lower and upper bounds
    float new_lower = std::max(lower, other.lower);
    float new_upper = std::min(upper, other.upper);

    // return the empty interval if the new lower bound is greater than the new upper bound
    if (new_lower > new_upper) {
        return {};
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


    return {new_lower, new_upper, new_left, new_right};
}

Interval SimpleInterval::complement() {
    const std::vector<SimpleInterval> resulting_intervals = {
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

bool SimpleInterval::is_empty() const {
    return lower == upper and (left == BorderType::OPEN or right == BorderType::OPEN);
}

bool SimpleInterval::operator==(const SimpleInterval &other) const {
    return lower == other.lower and upper == other.upper and left == other.left and right == other.right;
}

Interval SimpleInterval::difference_with(const SimpleInterval &other) const {

    // get the intersection of both atomic intervals
    SimpleInterval intersection = intersection_with(other);

    // if the intersection is empty, return the current atomic interval as interval
    if (intersection.is_empty()) {
        return Interval({*this});
    }

    // get the complement of the intersection
    Interval complement_of_intersection = intersection.complement();

    // initialize the difference vector
    std::vector<SimpleInterval> difference;

    // for every interval in the complement of the intersection
    for (auto interval: complement_of_intersection.intervals) {

        // intersect this with the current complement of the intersection
        SimpleInterval intersection_with_complement = intersection_with(interval);

        // if the intersection with the complement is not empty, append it to the difference vector
        if (!intersection_with_complement.is_empty()) {
            difference.push_back(intersection_with_complement);
        }
    }

    return Interval(difference);
}

std::string SimpleInterval::to_string() const {
    if (is_empty()) {
        return "∅";
    }
    char left_representation = left == BorderType::OPEN ? '(' : '[';
    char right_representation = right == BorderType::OPEN ? ')' : ']';
    return std::string(
            left_representation + std::to_string(lower) + ", " + std::to_string(upper) + right_representation);
}

bool SimpleInterval::contains(SimpleInterval &other) const {
    return intersection_with(other) == other;
}

bool SimpleInterval::contains(float element) const {
    auto singleton = SimpleInterval{element, element, BorderType::CLOSED, BorderType::CLOSED};
    return contains(singleton);
}

Interval SimpleInterval::difference_with(const Interval &other) const {
    Interval result;

    auto elementwise_differences = std::vector<Interval>{};

    for (auto atomic: other.intervals) {
        auto difference = difference_with(atomic);
        elementwise_differences.push_back(difference);
    }

    result.intervals.push_back(*this);
    for (const auto &elementwise_difference: elementwise_differences) {
        result = result.intersection_with(elementwise_difference);
    }

    return result;
}

Interval::Interval(const std::vector<SimpleInterval> &simple_intervals) {
    for (auto interval: simple_intervals) {
        if (!interval.is_empty()) {
            intervals.push_back(interval);
        }
    }
}

Interval Interval::make_disjoint() {
    Interval disjoint;
    Interval intersections;

    Interval current_disjoint;

    std::tie(disjoint, intersections) = split_into_disjoint_and_non_disjoint();

    while (!intersections.is_empty()) {
        std::tie(current_disjoint, intersections) = intersections.split_into_disjoint_and_non_disjoint();
        extend_vector(disjoint.intervals, current_disjoint.intervals);
    }

    disjoint.intervals.erase(unique(disjoint.intervals.begin(), disjoint.intervals.end()),
                             disjoint.intervals.end());

    return disjoint.simplify();
}

bool Interval::is_empty() const {
    return intervals.empty();
}

std::string Interval::to_string() const {
    if (is_empty()) {
        return "∅";
    }
    std::string result;
    for (size_t i = 0; i < intervals.size(); ++i) {
        result.append(intervals[i].to_string());
        if (i != intervals.size() - 1) {
            result.append(" u ");
        }
    }
    return result;
}

bool Interval::is_disjoint() const {
    for (auto combination: unique_combinations(intervals)) {
        SimpleInterval first = std::get<0>(combination);
        SimpleInterval second = std::get<1>(combination);
        if (!first.intersection_with(second).is_empty()) {
            return false;
        }
    }
    return true;
}

Interval Interval::intersection_with(SimpleInterval &simple) {
    std::vector<SimpleInterval> intersections;
    for (auto interval: intervals) {
        SimpleInterval intersection = interval.intersection_with(simple);
        if (!intersection.is_empty()) {
            intersections.push_back(intersection);
        }
    }
    return Interval(intersections);
}

std::tuple<Interval, Interval> Interval::split_into_disjoint_and_non_disjoint() {
    // initialize result for disjoint and non-disjoint sets
    Interval disjoint;
    Interval non_disjoint;

    // for every pair of atomics
    for (const auto &atomic_i: intervals) {

        // initialize the difference of A_i
        SimpleInterval difference = atomic_i;

        for (const auto &atomic_j: intervals) {

            // if the atomic intervals are the same, skip
            if (atomic_i == atomic_j) {
                continue;
            }

            // get the intersection of the atomic intervals
            auto intersection = atomic_i.intersection_with(atomic_j);

            // if the intersection is not empty, append it to the non-disjoint set
            if (!intersection.is_empty()) {
                non_disjoint.intervals.push_back(intersection);
            }

            difference = difference.difference_with(atomic_j).intervals[0];
        }

        disjoint.intervals.push_back(difference);
    }
    return std::make_tuple(disjoint, non_disjoint);
}

Interval::Interval() {
    intervals = {};
}

bool Interval::contains(SimpleInterval element) const {
    for (auto interval: intervals) {
        auto intersection = interval.intersection_with(element);
        element = element.difference_with(intersection).intervals[0];
        if (element.is_empty()) {
            return true;
        }
    }
    return false;
}

Interval Interval::intersection_with(const Interval &other) {
    Interval result = Interval();
    for (auto atomic_i: intervals) {
        for (auto atomic_j: other.intervals) {
            auto intersection = atomic_i.intersection_with(atomic_j);
            if (!intersection.is_empty()) {
                result.intervals.push_back(intersection);
            }
        }
    }
    return result;
}

Interval Interval::difference_with(const Interval &other) {
    Interval result = empty();
    for (auto atomic: intervals) {
        auto difference = atomic.difference_with(other);
        extend_vector(result.intervals, difference.intervals);
    }
    return result;
}

Interval Interval::simplify() {
    Interval result = Interval();
    Interval sorted = *this;

    std::sort(sorted.intervals.begin(), sorted.intervals.end(), by_lower_ascending());
    result.intervals.push_back(sorted.intervals[0]);

    for (auto current_atom = sorted.intervals.begin() + 1; current_atom != sorted.intervals.end(); ++current_atom) {
        auto last_atom = result.intervals.back();
        if (last_atom.upper == current_atom->lower &&
            !(last_atom.right == BorderType::OPEN and current_atom->left == BorderType::OPEN)) {
            result.intervals.pop_back();
            result.intervals.push_back(
                    SimpleInterval{last_atom.lower, current_atom->upper, last_atom.left, current_atom->right});
        } else {
            result.intervals.push_back(*current_atom);
        }
    }
    return result;
}


std::vector<std::tuple<SimpleInterval, SimpleInterval>> unique_combinations(
        const std::vector<SimpleInterval> &elements) {

    // initialize result
    std::vector<std::tuple<SimpleInterval, SimpleInterval>> combinations;

    // for every pair of elements
    for (std::size_t i = 0; i < elements.size(); ++i) {

        // get element from first vector
        SimpleInterval current_element1 = elements[i];
        for (std::size_t j = 0; j < i; ++j) {
            SimpleInterval current_element2 = elements[j];
            std::tuple<SimpleInterval, SimpleInterval> combination =
                    std::make_tuple(current_element1, current_element2);
            combinations.push_back(combination);
        }
    }
    return combinations;
}
