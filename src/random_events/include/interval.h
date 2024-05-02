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
inline BorderType intersect_borders(const BorderType border_1, const BorderType border_2) {
    return (border_1 == BorderType::OPEN || border_2 == BorderType::OPEN) ? BorderType::OPEN : BorderType::CLOSED;
}

/**
 * Logically t_complement a border.
 * @param border The borders to t_complement.
 * @return The t_complement a border.
 */
inline BorderType invert_border(const BorderType border) {
    return border == BorderType::OPEN ? BorderType::CLOSED : BorderType::OPEN;
}


/**
 * Class that represents an atomic interval.
 */
class SimpleInterval : public AbstractSimpleSet {
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


    SimpleInterval *intersection_with(const AbstractSimpleSet *other) const override;

    SimpleSetSet_t *complement() const override;

    bool contains(const ElementaryVariant *element) const override;

    bool is_empty() const override;

    /**
     * This method depends on the type of simple set and has to be overloaded.
     *
     * @param other The other simple set.
     * @return True if they are equal.
     */
    bool operator==(const AbstractSimpleSet &other) const override;

    bool operator==(const SimpleInterval &other) const;

    std::string *non_empty_to_string() const override;

    bool operator<(const AbstractSimpleSet &other) const override;

    /**
     * Compare two simple intervals. Simple intervals are ordered by lower bound. If the lower bound is equal, they are
     * ordered by upper bound.
     *
     * Note that border types are ignored in ordering.
     *
     * @param other The other interval
     * @return True if this interval is less than the other interval.
     */
    bool operator<(const SimpleInterval &other) const;


    bool operator<=(const AbstractSimpleSet &other) const override;

    /**
    * Compare two simple intervals. Simple intervals are ordered by lower bound. If the lower bound is equal, they are
    * ordered by upper bound.
    *
    * Note that border types are ignored in ordering.
    *
    * @param other The other interval
    * @return True if this interval is less or equal to the other interval.
    */
    bool operator<=(const SimpleInterval &other) const;
};


/**
 * Hash function for simple intervals.
 */
namespace std {
    template<>
    struct hash<SimpleInterval> {
        size_t operator()(const SimpleInterval &interval) const {
            return std::hash<float>()(interval.lower) ^ std::hash<float>()(interval.upper) ^ std::hash<int>()(
                    static_cast<int>(interval.left)) ^ std::hash<int>()(static_cast<int>(interval.right));
        }
    };
}


/**
 * Class that represents a composite interval.
 * An interval is an (automatically simplified) union of simple simple_sets.
 */
class Interval : public AbstractCompositeSet {
public:
    Interval() = default;

    //
    // explicit Interval(const SimpleSetType<SimpleInterval> &simple_sets) {
    //     this->simple_sets = simple_sets;
    //     this->empty_simple_set_ptr = &simple_interval;
    // }
    //
    // explicit Interval(const SimpleInterval &simple_interval) {
    //     this->simple_sets.insert(simple_interval);
    //     this->empty_simple_set_ptr = &this->simple_interval;
    // }

    ~Interval() override;

    [[nodiscard]] Interval *simplify() const override;

    Interval *make_new_empty(AbstractAllElements *all_elements) const override;

    Interval *make_new(std::set<AbstractSimpleSet *> *simple_sets_,
                       AbstractAllElements *all_elements_) const override;


    /**
     * The empty simple interval.
     */
    SimpleInterval simple_interval;
};

// inline Interval closed(const float lower, const float upper) {
//  return Interval(
//   SimpleSetType<SimpleInterval>{SimpleInterval{lower, upper, BorderType::CLOSED, BorderType::CLOSED}});
// }
//
// inline Interval open(const float lower, const float upper) {
//  return Interval(SimpleSetType<SimpleInterval>{SimpleInterval{lower, upper, BorderType::OPEN, BorderType::OPEN}});
// }
//
// inline Interval open_closed(const float lower, const float upper) {
//  return Interval(SimpleSetType<SimpleInterval>{SimpleInterval{lower, upper, BorderType::OPEN, BorderType::CLOSED}});
// }
//
// inline Interval closed_open(const float lower, const float upper) {
//  return Interval(SimpleSetType<SimpleInterval>{SimpleInterval{lower, upper, BorderType::CLOSED, BorderType::OPEN}});
// }
//
// inline Interval singleton(const float value) {
//  return Interval(
//   SimpleSetType<SimpleInterval>{SimpleInterval{value, value, BorderType::CLOSED, BorderType::CLOSED}});
// }
//
// inline Interval empty() {
//  return Interval(SimpleSetType<SimpleInterval>{});
// }
//
// inline Interval reals() {
//  return Interval(SimpleSetType<SimpleInterval>{
//   SimpleInterval{
//    -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(),
//    BorderType::OPEN, BorderType::OPEN
//   }
//  });
// }
