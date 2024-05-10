#include <vector>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include "interval.h"
#include "sigma_algebra.h"


template<typename Orderable_T>
AbstractSimpleSetPtr_t SimpleInterval<Orderable_T>::intersection_with(const AbstractSimpleSetPtr_t &other) {
    const auto derived_other = (SimpleInterval<Orderable_T> *) other.get();

    // get the new lower and upper bounds
    const Orderable_T new_lower = std::max(lower, derived_other->lower);
    const Orderable_T new_upper = std::min(upper, derived_other->upper);

    // return the empty interval if the new lower bound is greater than the new upper bound
    if (new_lower > new_upper) {
        return make_shared();
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

    return make_shared(new_lower, new_upper, new_left, new_right);
}

template<typename Orderable_T>
template<typename... Args>
std::shared_ptr<SimpleInterval<Orderable_T>> SimpleInterval<Orderable_T>::make_shared(Args &&... args) {
    return std::make_shared<SimpleInterval<Orderable_T>>(std::forward<Args>(args)...);
}

template<typename Orderable_T>
template<typename... Args>
std::shared_ptr<Interval<Orderable_T>> Interval<Orderable_T>::make_shared(Args &&... args) {
    return std::make_shared<Interval<Orderable_T>>(std::forward<Args>(args)...);
}


template<typename Orderable_T>
SimpleSetSetPtr_t SimpleInterval<Orderable_T>::complement() {
    auto resulting_intervals = make_shared_simple_set_set();

    // if the interval is the real line, return an empty set
    if (lower == -std::numeric_limits<Orderable_T>::infinity() and
        upper == std::numeric_limits<Orderable_T>::infinity()) {
        return resulting_intervals;
    }

    // if the interval is empty, return the real line
    if (is_empty()) {
        resulting_intervals->insert(
                SimpleInterval<Orderable_T>::make_shared(-std::numeric_limits<Orderable_T>::infinity(), std::numeric_limits<Orderable_T>::infinity(),
                            BorderType::OPEN, BorderType::OPEN));
        return resulting_intervals;
    }

    // if the interval has nothing left
    if (upper < std::numeric_limits<Orderable_T>::infinity()) {
        resulting_intervals->insert(
                SimpleInterval<Orderable_T>::make_shared(upper, std::numeric_limits<Orderable_T>::infinity(), invert_border(right),
                                            BorderType::OPEN));
    }

    if (lower > -std::numeric_limits<Orderable_T>::infinity()) {
        resulting_intervals->insert(
                SimpleInterval<Orderable_T>::make_shared(-std::numeric_limits<Orderable_T>::infinity(), lower, BorderType::OPEN,
                                            invert_border(left)));
    }

    return resulting_intervals;
}

template<typename Orderable_T>
bool SimpleInterval<Orderable_T>::contains(const ElementaryVariant *element) {
    return false;
}

template<typename Orderable_T>
bool SimpleInterval<Orderable_T>::is_empty() {
    return lower > upper or (lower == upper and (left == BorderType::OPEN or right == BorderType::OPEN));
}

template<typename Orderable_T>
bool SimpleInterval<Orderable_T>::operator==(const AbstractSimpleSet &other) {
    auto derived_other = (SimpleInterval *) &other;
    return *this == *derived_other;
}


template<typename Orderable_T>
bool SimpleInterval<Orderable_T>::operator==(const SimpleInterval<Orderable_T> &other) {
    return lower == other.lower and upper == other.upper and left == other.left and right == other.right;
}

template<typename Orderable_T>
std::string *SimpleInterval<Orderable_T>::non_empty_to_string() {
    const char left_representation = left == BorderType::OPEN ? '(' : '[';
    const char right_representation = right == BorderType::OPEN ? ')' : ']';
    return new std::string(
            left_representation + std::to_string(lower) + ", " + std::to_string(upper) + right_representation);
}


template<typename Orderable_T>
bool SimpleInterval<Orderable_T>::operator<(const SimpleInterval<Orderable_T> &other) {
    if (lower == other.lower) {
        return upper < other.upper;
    }
    return lower < other.lower;
}

template<typename Orderable_T>
bool SimpleInterval<Orderable_T>::operator<(const AbstractSimpleSet &other) {
    const auto derived_other = (SimpleInterval<Orderable_T> *) &other;
    return *this < *derived_other;
}

template<typename Orderable_T>
bool SimpleInterval<Orderable_T>::operator<=(const SimpleInterval<Orderable_T> &other) {
    if (lower == other.lower) {
        return upper <= other.upper;
    }
    return lower <= other.lower;
}

template<typename Orderable_T>
bool SimpleInterval<Orderable_T>::operator<=(const AbstractSimpleSet &other) {
    const auto derived_other = (SimpleInterval<Orderable_T> *) &other;
    return *this <= *derived_other;
}


template<typename Orderable_T>
bool SimpleInterval<Orderable_T>::contains(Orderable_T element) const {
    if (left == BorderType::OPEN and element <= lower) {
        return false;
    }

    if (right == BorderType::OPEN and element >= upper) {
        return false;
    }

    if (left == BorderType::CLOSED and element < lower) {
        return false;
    }

    if (right == BorderType::CLOSED and element > upper) {
        return false;
    }


    return true;
}


template<typename Orderable_T>
Interval<Orderable_T>::~Interval() {
    simple_sets->clear();
}

template<typename Orderable_T>
AbstractCompositeSetPtr_t Interval<Orderable_T>::simplify() {
    auto result = make_shared_simple_set_set();
    bool first_iteration = true;

    for (const auto &current_simple_set: *simple_sets) {
        auto current_simple_interval = std::static_pointer_cast<SimpleInterval<Orderable_T>>(current_simple_set);

        // if this is the first iteration, just copy the interval
        if (first_iteration) {
            result->insert(current_simple_interval);
            first_iteration = false;
            continue;
        }

        auto last_simple_interval = std::dynamic_pointer_cast<SimpleInterval<Orderable_T>>(*result->rbegin());

        if (last_simple_interval->upper == current_simple_interval->lower &&
            !(last_simple_interval->right == BorderType::OPEN and current_simple_interval->left == BorderType::OPEN)) {
            last_simple_interval->upper = current_simple_interval->upper;
            last_simple_interval->right = current_simple_interval->right;
        } else {
            result->insert(current_simple_interval);
        }
    }

    return Interval::make_shared(result);
}

template<typename Orderable_T>
AbstractCompositeSetPtr_t Interval<Orderable_T>::make_new_empty() {
    return Interval::make_shared();
}

template<typename Orderable_T>
Orderable_T Interval<Orderable_T>::lower() const {
    return std::dynamic_pointer_cast<SimpleInterval<Orderable_T>>(*simple_sets->begin())->lower;
}

template<typename Orderable_T>
Orderable_T Interval<Orderable_T>::upper() const {
    return std::dynamic_pointer_cast<SimpleInterval<Orderable_T>>(*simple_sets->rbegin())->upper;
}

template<typename Orderable_T>
bool Interval<Orderable_T>::contains(Orderable_T element) const {
    for (const auto &simple_set: *simple_sets) {
        auto simple_interval = std::static_pointer_cast<SimpleInterval<Orderable_T>>(simple_set);
        if (simple_interval->contains(element)) {
            return true;
        }
    }
    return false;
}

