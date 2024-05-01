#include <vector>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "interval.h"
#include "sigma_algebra.h"


SimpleInterval::SimpleInterval(const float lower, const float upper, const BorderType left,
                               const BorderType right) : lower(lower),
                                                         upper(upper),
                                                         left(left),
                                                         right(right) {
    if (lower > upper) { throw std::invalid_argument("Lower bound must be less than or equal to upper bound."); }
}

SimpleInterval *SimpleInterval::intersection_with(const AbstractSimpleSet *other) const {
    const auto derived_other = (SimpleInterval*) other;

    // get the new lower and upper bounds
    const float new_lower = std::max(lower, derived_other->lower);
    const float new_upper = std::min(upper, derived_other->upper);

    // return the empty interval if the new lower bound is greater than the new upper bound
    if (new_lower > new_upper) {
        return new SimpleInterval();
    }

    // initialize the new borders
    BorderType new_left;
    BorderType new_right;

    // if the lower bounds are equal, intersect the borders
    if (lower == derived_other->lower) {
        new_left = intersect_borders(left, derived_other->left);
    } else {
        // else take the border of the interval with the lower bound
        new_left = lower == new_lower ? left : derived_other->left;
    }

    // if the upper bounds are equal, intersect the borders
    if (upper == derived_other->upper) {
        new_right = intersect_borders(right, derived_other->right);
    } else {
        // else take the border of the interval with the upper bound
        new_right = upper == new_upper ? right : derived_other->right;
    }

    return new SimpleInterval{new_lower, new_upper, new_left, new_right};
}

std::set<AbstractSimpleSet *> *SimpleInterval::complement() const {
    auto resulting_intervals = new std::set<AbstractSimpleSet *>;

    if (lower == -std::numeric_limits<float>::infinity() and upper == std::numeric_limits<float>::infinity()) {
        return resulting_intervals;
    }

    if (lower == -std::numeric_limits<float>::infinity()) {
        resulting_intervals->insert(new SimpleInterval{
            upper, std::numeric_limits<float>::infinity(),
            invert_border(right), BorderType::OPEN
        });
    }

    if (upper == std::numeric_limits<float>::infinity()) {
        resulting_intervals->insert(new SimpleInterval{
            -std::numeric_limits<float>::infinity(), lower,
            BorderType::OPEN, invert_border(left)
        });
    }

    return resulting_intervals;
}

bool SimpleInterval::contains(const ElementaryVariant *element) const {
    return false;
}

bool SimpleInterval::is_empty() const {
    return lower > upper or (lower == upper and (left == BorderType::OPEN or right == BorderType::OPEN));
}


bool SimpleInterval::operator==(const AbstractSimpleSet *other) const {
    auto derived_other = (SimpleInterval*) other;
    return this == derived_other;
}

bool SimpleInterval::operator==(const SimpleInterval* other) const {
    return lower == other->lower and upper == other->upper and left == other->left and right ==
       other->right;
}

bool SimpleInterval::operator==(const SimpleInterval &other) const {
    return *this == &other;
}

std::string *SimpleInterval::non_empty_to_string() const {
    const char left_representation = left == BorderType::OPEN ? '(' : '[';
    const char right_representation = right == BorderType::OPEN ? ')' : ']';
    return new std::string(
        left_representation + std::to_string(lower) + ", " + std::to_string(upper) + right_representation);
}

bool SimpleInterval::operator<(const AbstractSimpleSet *other) const {
    //cast other to simple interval
    const auto derived_other = (SimpleInterval*) other;

    if (lower == derived_other->lower) {
        return upper < derived_other->upper;
    }
    return lower < derived_other->lower;
}

bool SimpleInterval::operator<=(const AbstractSimpleSet *other) const {
    //cast other to simple interval
    const auto derived_other = (SimpleInterval*) other;

    if (lower == derived_other->lower) {
        return upper <= derived_other->upper;
    }
    return lower <= derived_other->lower;
}

Interval::~Interval() {
    simple_sets.clear();
}

Interval *Interval::simplify() const {
    std::set<SimpleInterval *> result;

    // // convert to vector and cast to SimpleInterval
    // auto simple_set_vector = std::vector<SimpleInterval>();
    // for (const auto simple_set : simple_sets) {
    //     simple_set_vector.emplace_back(*simple_set);
    // }
    //
    // //std::sort(simple_set_vector.begin(), simple_set_vector.end());
    // result.push_back(simple_set_vector[0]);

    bool first_iteration = true;

    for (const auto current_simple_set: simple_sets) {
        auto current_simple_interval = (SimpleInterval*) current_simple_set;

        // if this is the first iteration, just copy the interval
        if (first_iteration) {
            result.insert(current_simple_interval);
            first_iteration = false;
            continue;
        }

        auto last_simple_interval = *result.end();

        if (last_simple_interval->upper == current_simple_interval->lower &&
            !(last_simple_interval->right == BorderType::OPEN and current_simple_interval->left == BorderType::OPEN)) {
            last_simple_interval->upper = current_simple_interval->upper;
            last_simple_interval->right = current_simple_interval->right;
            // result.emplace_back(last_simple_interval.lower, current_simple_interval->upper, last_simple_interval.left,
            //                     current_simple_interval->right);
        } else {
            result.insert(current_simple_interval);
        }
    }

    return nullptr;
    //return new Interval(result, all_elements);
}

Interval * Interval::make_new_empty(AbstractAllElements *all_elements) const {
    return new Interval();
}

Interval * Interval::make_new(std::set<AbstractSimpleSet *> *simple_sets_, AbstractAllElements *all_elements_) const {
    return new Interval();
}
