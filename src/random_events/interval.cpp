#include <limits>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include "interval.h"
#include "sigma_algebra.h"

SimpleInterval::SimpleInterval(const float lower, const float upper, const BorderType left, const BorderType right)
        : lower(lower), upper(upper), left(left), right(right) {
    if (lower > upper) { throw std::invalid_argument("Lower bound must be less than or equal to upper bound."); }
}

AbstractSimpleSetPtr_t SimpleInterval::intersection_with(const AbstractSimpleSetPtr_t &other) {
    const auto derived_other = (SimpleInterval *) other.get();

    // get the new lower and upper bounds
    const float new_lower = std::max(lower, derived_other->lower);
    const float new_upper = std::min(upper, derived_other->upper);

    // return the empty interval if the new lower bound is greater than the new upper bound
    if (new_lower > new_upper) {
        return make_shared_simple_interval();
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

    return make_shared_simple_interval(new_lower, new_upper, new_left, new_right);
}

SimpleSetSetPtr_t SimpleInterval::complement() {
    auto resulting_intervals = make_shared_simple_set_set();

    // if the interval is the real line, return an empty set
    if (lower == -std::numeric_limits<float>::infinity() and upper == std::numeric_limits<float>::infinity()) {
        return resulting_intervals;
    }

    // if the interval is empty, return the real line
    if (is_empty()) {
        resulting_intervals->insert(make_shared_simple_interval(-std::numeric_limits<float>::infinity(),
                                                                std::numeric_limits<float>::infinity(),
                                                                BorderType::OPEN, BorderType::OPEN));
        return resulting_intervals;
    }

    // if the interval has nothing left
    if (upper < std::numeric_limits<float>::infinity()) {
        resulting_intervals->insert(
                make_shared_simple_interval(upper, std::numeric_limits<float>::infinity(), invert_border(right),
                                            BorderType::OPEN));
    }

    if (lower > -std::numeric_limits<float>::infinity()) {
        resulting_intervals->insert(
                make_shared_simple_interval(-std::numeric_limits<float>::infinity(), lower, BorderType::OPEN,
                                            invert_border(left)));
    }

    return resulting_intervals;
}

bool SimpleInterval::contains(const ElementaryVariant *element) {
    return false;
}

bool SimpleInterval::is_empty() {
    return lower > upper or (lower == upper and (left == BorderType::OPEN or right == BorderType::OPEN));
}


bool SimpleInterval::operator==(const AbstractSimpleSet &other) {
    auto derived_other = (SimpleInterval *) &other;
    return *this == *derived_other;
}


bool SimpleInterval::operator==(const SimpleInterval &other) {
    return lower == other.lower and upper == other.upper and left == other.left and right == other.right;
}

std::string *SimpleInterval::non_empty_to_string() {
    const char left_representation = left == BorderType::OPEN ? '(' : '[';
    const char right_representation = right == BorderType::OPEN ? ')' : ']';
    return new std::string(
            left_representation + std::to_string(lower) + ", " + std::to_string(upper) + right_representation);
}


bool SimpleInterval::operator<(const SimpleInterval &other) {
    if (lower == other.lower) {
        return upper < other.upper;
    }
    return lower < other.lower;
}

bool SimpleInterval::operator<(const AbstractSimpleSet &other) {
    const auto derived_other = (SimpleInterval *) &other;
    return *this < *derived_other;
}


bool SimpleInterval::operator<=(const SimpleInterval &other) {
    if (lower == other.lower) {
        return upper <= other.upper;
    }
    return lower <= other.lower;
}

bool SimpleInterval::operator<=(const AbstractSimpleSet &other) {
    const auto derived_other = (SimpleInterval *) &other;
    return *this <= *derived_other;
}


Interval::~Interval() {
    simple_sets->clear();
}

AbstractCompositeSetPtr_t Interval::simplify() {
    auto result = make_shared_simple_set_set();
    bool first_iteration = true;

    for (const auto &current_simple_set: *simple_sets) {
        auto current_simple_interval = std::static_pointer_cast<SimpleInterval>(current_simple_set);

        // if this is the first iteration, just copy the interval
        if (first_iteration) {
            result->insert(current_simple_interval);
            first_iteration = false;
            continue;
        }

        auto last_simple_interval = std::dynamic_pointer_cast<SimpleInterval>(*result->rbegin());

        if (last_simple_interval->upper == current_simple_interval->lower &&
            !(last_simple_interval->right == BorderType::OPEN and current_simple_interval->left == BorderType::OPEN)) {
            last_simple_interval->upper = current_simple_interval->upper;
            last_simple_interval->right = current_simple_interval->right;
        } else {
            result->insert(current_simple_interval);
        }
    }

    return make_shared_interval(result);
}

AbstractCompositeSetPtr_t Interval::make_new_empty() {
    return make_shared_interval();
}

