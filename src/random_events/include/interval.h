#pragma once
#include "sigma_algebra.h"
#include <unordered_set>

/**
 * Enum for border types of simple_sets.
 */
enum class BorderType {

    /**
     * Open indicates that a value is included in the interval.
     */
    OPEN,

    /**
     * Close indicates that a value is excluded in the interval.
     */
    CLOSED

};

/**
 * Logically intersect borders.
 * @param border_1 One of the borders to intersect.
 * @param border_2 The other border to intersect.
 * @return The intersection of the borders.
 */
inline BorderType intersect_borders(BorderType border_1, BorderType border_2) {
    return (border_1 == BorderType::OPEN || border_2 == BorderType::OPEN) ? BorderType::OPEN : BorderType::CLOSED;
}

/**
 * Logically complement a border.
 * @param border The borders to complement.
 * @return The complement a border.
 */
inline BorderType invert_border(BorderType border) {
    return border == BorderType::OPEN ? BorderType::CLOSED : BorderType::OPEN;
}

class Interval; // Forward declaration

/**
 * Class that represents an atomic interval.
 */
class SimpleInterval : public SimpleSetWrapper<Interval, SimpleInterval, float> {
public:
    /**
     * The lower value.
     */
    float lower = 0;

    /**
     * The upper value.
     */
    float upper = 0;

    /**
     * THe left border type.
     */
    BorderType left = BorderType::OPEN;

    /**
     * The right border type.
     */
    BorderType right = BorderType::OPEN;

    /**
     * Construct an atomic interval.
     */
    explicit SimpleInterval(float lower = 0, float upper = 0, BorderType left = BorderType::OPEN,
                            BorderType right = BorderType::OPEN);

    static SimpleInterval simple_set_empty_set() {
        return SimpleInterval();
    }

    [[nodiscard]] SimpleInterval simple_set_intersection_with(const SimpleInterval &other) const;

    [[nodiscard]] Interval simple_set_complement() const;

    [[nodiscard]] bool simple_set_contains(const float &element) const;

    [[nodiscard]] bool simple_set_is_empty() const;

    /**
     * This method depends on the type of simple set and has to be overloaded.
     *
     * @param other The other simple set.
     * @return True if they are equal.
     */
    bool operator==(const SimpleInterval &other) const;

    std::string to_string();

    explicit operator std::string();
};

/**
 * Hash function for simple sets.
 */
namespace std {
    template <> struct hash<SimpleInterval>
    {
        size_t operator()(const SimpleInterval& interval) const
        {
            return std::hash<float>()(interval.lower) ^ std::hash<float>()(interval.upper) ^ std::hash<int>()(
                    static_cast<int>(interval.left)) ^ std::hash<int>()(static_cast<int>(interval.right));
        }
    };
}

/**
 * Struct for sorting a composite interval by lower value.
 */
struct by_lower_ascending {
    bool operator()(const SimpleInterval &a, const SimpleInterval &b) const {
        if (a.lower == b.lower) {
            return a.upper < b.upper;
        }
        return a.lower < b.lower;
    }
};

/**
 * Struct for sorting a composite interval by upper value.
 */
struct by_upper_ascending {
    bool operator()(const SimpleInterval &a, const SimpleInterval &b) const {
        if (a.upper == b.upper) {
            return a.lower < b.lower;
        }
        return a.upper < b.upper;
    }
};

/**
 * Extend a vector with another vector.
 * @tparam T The type of the vector.
 * @param first The first vector.
 * @param second The second vector.
 */
template<typename T>
void extend_vector(std::vector<T> &first, const std::vector<T> &second) {
    first.reserve(first.size() + distance(second.begin(), second.end()));
    first.insert(first.end(), second.begin(), second.end());
}

/**
 * Class that represents a composite interval.
 * An interval is an (automatically simplified) union of simple simple_sets.
 */
class Interval : public CompositeSetWrapper<Interval, SimpleInterval, float> {

public:

    Interval() = default;

    explicit Interval(const SimpleSetType<SimpleInterval> &simple_sets) {
        this->simple_sets = simple_sets;
    }

    Interval composite_set_simplify();

//
//    /**
//     * Form the difference with another composite set.
//     *
//     * This difference is not guaranteed disjoint.
//     *
//     * @param other The other composite set.
//     * @return The difference as composite set.
//     */
//    Interval difference_with(const Interval &other);
//
//    /**
//     * Split this composite set into disjoint and non-disjoint parts.
//     *
//     * This method is required for making the union of composite sets disjoint.
//     * The partitioning is done by removing every other simple set from every simple set.
//     * The purified simple sets are then disjoint by definition and the pairwise intersections are not disjoint yet.
//     *
//     * @return A tuple of disjoint and non-disjoint composite sets.
//     */
//    std::tuple<Interval, Interval> split_into_disjoint_and_non_disjoint();
//

//
//    /**
//     * Check if a simple set is contained in the composite set.
//     * @param element The element to check.
//     * @return  True if the element is contained in the composite set.
//     */
//    [[nodiscard]] bool contains(SimpleInterval element) const;

};

inline Interval closed(float lower, float upper) {
    return Interval(SimpleSetType<SimpleInterval>{SimpleInterval{lower, upper, BorderType::CLOSED, BorderType::CLOSED}});
}

inline Interval open(float lower, float upper) {
    return Interval(SimpleSetType<SimpleInterval>{SimpleInterval{lower, upper, BorderType::OPEN, BorderType::OPEN}});
}

inline Interval open_closed(float lower, float upper) {
    return Interval(SimpleSetType<SimpleInterval>{SimpleInterval{lower, upper, BorderType::OPEN, BorderType::CLOSED}});
}

inline Interval closed_open(float lower, float upper) {
    return Interval(SimpleSetType<SimpleInterval>{SimpleInterval{lower, upper, BorderType::CLOSED, BorderType::OPEN}});
}

inline Interval singleton(float value) {
    return Interval(SimpleSetType<SimpleInterval>{SimpleInterval{value, value, BorderType::CLOSED, BorderType::CLOSED}});
}

inline Interval empty() {
    return Interval(SimpleSetType<SimpleInterval>{});
}

inline Interval reals() {
    return Interval(SimpleSetType<SimpleInterval>{
            SimpleInterval{-std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(),
                           BorderType::OPEN, BorderType::OPEN}});
}