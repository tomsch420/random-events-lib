#pragma once

#include "sigma_algebra.h"
#include <memory>
#include <utility>
#include <limits>


typedef double DefaultOrderable_T;

//FORWARD DECLARE
template<typename Orderable_T = DefaultOrderable_T>
class SimpleInterval;
template<typename  Orderable_T = DefaultOrderable_T>
class Interval;

// TYPEDEFS
template<typename Orderable_T>
using SimpleIntervalPtr_t = std::shared_ptr<SimpleInterval<Orderable_T>>;

template<typename Orderable_T>
using IntervalPtr_t = std::shared_ptr<Interval<Orderable_T>>;



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
 * Class that represents a simple interval.
 */
template<typename Orderable_T>
class SimpleInterval : public AbstractSimpleSet {
public:
    /**
     * The lower value.
     */
    Orderable_T lower = 0;

    /**
     * The upper value.
     */
    Orderable_T upper = 0;

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
    explicit SimpleInterval(Orderable_T lower = 0, Orderable_T upper = 0, BorderType left = BorderType::OPEN,
                            BorderType right = BorderType::OPEN) {
        this->lower = lower;
        this->upper = upper;
        this->left = left;
        this->right = right;
    }


    inline AbstractSimpleSetPtr_t intersection_with(const AbstractSimpleSetPtr_t &other) override;

    SimpleSetSetPtr_t complement() override;

    bool contains(const ElementaryVariant *element) override;

    bool contains(Orderable_T element) const;

    bool is_empty() override;

    /**
     * This method depends on the type of simple set and has to be overloaded.
     *
     * @param other The other simple set.
     * @return True if they are equal.
     */
    bool operator==(const AbstractSimpleSet &other) override;

    bool operator==(const SimpleInterval &other);

    std::string *non_empty_to_string() override;

    bool operator<(const AbstractSimpleSet &other) override;

    /**
     * Compare two simple intervals. Simple intervals are ordered by lower bound. If the lower bound is equal, they are
     * ordered by upper bound.
     *
     * Note that border types are ignored in ordering.
     *
     * @param other The other interval
     * @return True if this interval is less than the other interval.
     */
    bool operator<(const SimpleInterval &other);


    bool operator<=(const AbstractSimpleSet &other) override;

    /**
    * Compare two simple intervals. Simple intervals are ordered by lower bound. If the lower bound is equal, they are
    * ordered by upper bound.
    *
    * Note that border types are ignored in ordering.
    *
    * @param other The other interval
    * @return True if this interval is less or equal to the other interval.
    */
    bool operator<=(const SimpleInterval &other);

    template<typename... Args>
    static SimpleIntervalPtr_t<Orderable_T> make_shared(Args &&... args);

};
template class SimpleInterval<double>;

/**
 * Hash function for simple intervals.
 */
namespace std {
    template<typename  Orderable_T>
    struct hash<SimpleInterval<Orderable_T>> {
        size_t operator()(const SimpleInterval<Orderable_T> &interval) const {
            return std::hash<Orderable_T>()(interval.lower) ^ std::hash<Orderable_T>()(interval.upper) ^
                   std::hash<int>()(static_cast<int>(interval.left)) ^
                   std::hash<int>()(static_cast<int>(interval.right));
        }
    };
}

/**
 * Class that represents a composite interval.
 * An interval is an (automatically simplified) union of simple simple_sets.
 */
template<typename  Orderable_T>
class Interval : public AbstractCompositeSet {
public:

    Interval(){
        this->simple_sets = make_shared_simple_set_set();
    };

    explicit Interval(const SimpleSetSetPtr_t &simple_sets_) {
        this->simple_sets = simple_sets_;
    }


    explicit Interval(const SimpleInterval<Orderable_T> &simple_interval) {
        simple_sets->insert(std::make_shared<SimpleInterval<Orderable_T>>(simple_interval));
    }

    explicit Interval(SimpleSetSetPtr_t &simple_sets_) {
        this->simple_sets = simple_sets_;
    }

    ~Interval() override;

    AbstractCompositeSetPtr_t simplify() override;

    AbstractCompositeSetPtr_t make_new_empty() override;

    Orderable_T lower() const;

    Orderable_T upper() const;

    bool contains(Orderable_T element) const;

    template<typename... Args>
    static std::shared_ptr<Interval<Orderable_T>> make_shared(Args &&... args);

};

template class Interval<double>;

template<typename Orderable_T = DefaultOrderable_T>
inline IntervalPtr_t<Orderable_T> closed(const Orderable_T lower, const Orderable_T upper) {
    auto interval = SimpleInterval<Orderable_T>::make_shared(lower, upper, BorderType::CLOSED, BorderType::CLOSED);
    auto intervals = make_shared_simple_set_set();
    intervals->insert(interval);
    return Interval<Orderable_T>::make_shared(intervals);
}

template<typename Orderable_T = DefaultOrderable_T>
inline IntervalPtr_t<Orderable_T> open(const Orderable_T lower, const Orderable_T upper) {
    auto interval = SimpleInterval<Orderable_T>::make_shared(lower, upper, BorderType::OPEN, BorderType::OPEN);
    auto intervals = make_shared_simple_set_set();
    intervals->insert(interval);
    return Interval<Orderable_T>::make_shared(intervals);
}

template<typename Orderable_T = DefaultOrderable_T>
inline IntervalPtr_t<Orderable_T>  open_closed(const Orderable_T lower, const Orderable_T upper) {
    auto interval = SimpleInterval<Orderable_T>::make_shared(lower, upper, BorderType::OPEN, BorderType::CLOSED);
    auto intervals = make_shared_simple_set_set();
    intervals->insert(interval);
    return Interval<Orderable_T>::make_shared(intervals);
}

template<typename Orderable_T = DefaultOrderable_T>
inline IntervalPtr_t<Orderable_T>  closed_open(const Orderable_T lower, const Orderable_T upper) {
    auto interval = SimpleInterval<Orderable_T>::make_shared(lower, upper, BorderType::CLOSED, BorderType::OPEN);
    auto intervals = make_shared_simple_set_set();
    intervals->insert(interval);
    return Interval<Orderable_T>::make_shared(intervals);
}

template<typename Orderable_T = DefaultOrderable_T>
inline IntervalPtr_t<Orderable_T>  singleton(const Orderable_T value) {
    auto interval = SimpleInterval<Orderable_T>::make_shared(value, value, BorderType::CLOSED, BorderType::CLOSED);
    auto intervals = make_shared_simple_set_set();
    intervals->insert(interval);
    return Interval<Orderable_T>::make_shared(intervals);
}

template<typename Orderable_T = DefaultOrderable_T>
inline IntervalPtr_t<Orderable_T>  empty() {
    auto intervals = make_shared_simple_set_set();
    return Interval<Orderable_T>::make_shared(intervals);
}

template<typename Orderable_T = DefaultOrderable_T>
inline IntervalPtr_t<Orderable_T> reals() {
    auto interval = SimpleInterval<Orderable_T>::make_shared(-std::numeric_limits<Orderable_T>::infinity(),
                                                std::numeric_limits<Orderable_T>::infinity(), BorderType::OPEN,
                                                BorderType::OPEN);
    auto intervals = make_shared_simple_set_set();
    intervals->insert(interval);
    return Interval<Orderable_T>::make_shared(intervals);
}
