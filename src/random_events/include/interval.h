#pragma once

#include "sigma_algebra.h"
#include <memory>
#include <utility>

//FORWARD DECLARE
class SimpleInterval;
class RealLine;
class Interval;

// TYPEDEFS
typedef std::shared_ptr<SimpleInterval> SimpleIntervalPtr_t;

template<typename... Args>
std::shared_ptr<SimpleInterval> make_shared_simple_interval(Args &&... args) {
    return std::make_shared<SimpleInterval>(std::forward<Args>(args)...);
}

typedef std::shared_ptr<Interval> IntervalPtr_t;

template<typename... Args>
std::shared_ptr<Interval> make_shared_interval(Args &&... args) {
    return std::make_shared<Interval>(std::forward<Args>(args)...);
}

using RealLinePtr_t = std::shared_ptr<RealLine>;

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


    AbstractSimpleSetPtr_t intersection_with(const AbstractSimpleSetPtr_t &other) override;

    SimpleSetSetPtr_t complement() override;

    bool contains(const ElementaryVariant *element) override;

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

    RealLinePtr_t all_elements;

    AbstractAllElementsPtr_t get_all_elements() override;

};


/**
 * Hash function for simple intervals.
 */
namespace std {
    template<>
    struct hash<SimpleInterval> {
        size_t operator()(const SimpleInterval &interval) const {
            return std::hash<float>()(interval.lower) ^ std::hash<float>()(interval.upper) ^
                   std::hash<int>()(static_cast<int>(interval.left)) ^
                   std::hash<int>()(static_cast<int>(interval.right));
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

    RealLinePtr_t all_elements;

    AbstractAllElementsPtr_t get_all_elements() override;

    explicit Interval(const SimpleSetSetPtr_t &simple_sets_) {
        this->simple_sets = simple_sets_;
        empty_simple_set_ptr = std::make_shared<SimpleInterval>(simple_interval);
    }

    explicit Interval(const RealLinePtr_t& all_elements_) {
        this->simple_sets = make_shared_simple_set_set();
        empty_simple_set_ptr = std::make_shared<SimpleInterval>(simple_interval);
        this->all_elements = all_elements_;
    }

    explicit Interval(const SimpleInterval &simple_interval) {
        simple_sets->insert(std::make_shared<SimpleInterval>(simple_interval));
        empty_simple_set_ptr = std::make_shared<SimpleInterval>(simple_interval);
    }

    explicit Interval(SimpleSetSetPtr_t &simple_sets_, const RealLinePtr_t& all_elements_) {
        this->simple_sets = simple_sets_;
        empty_simple_set_ptr = std::make_shared<SimpleInterval>(simple_interval);
        this->all_elements = all_elements_;
    }

    ~Interval() override;

    AbstractCompositeSetPtr_t simplify() override;

    AbstractCompositeSetPtr_t make_new_empty(const AbstractAllElementsPtr_t& all_elements_) override;

    AbstractCompositeSetPtr_t
    make_new(const SimpleSetSetPtr_t& simple_sets_, const AbstractAllElementsPtr_t& all_elements_) override;


    /**
     * The empty simple interval.
     */
    SimpleInterval simple_interval;
};


/**
 * Class for holding an unfinished interval that represents the real line.
 */
class RealLine : public AbstractAllElements {

    IntervalPtr_t all_elements;

public:
    RealLine() {
        auto reals = make_shared_simple_interval(-std::numeric_limits<float>::infinity(),
                                                         std::numeric_limits<float>::infinity(), BorderType::OPEN,
                                                         BorderType::OPEN);
        auto intervals = make_shared_simple_set_set();
        intervals->insert(reals);
        all_elements = make_shared_interval(intervals);
    }

    /**
    * The real line that can be used as all_elements member in intervals.
    */
    static RealLinePtr_t real_line_ptr;

};


inline IntervalPtr_t closed(const float lower, const float upper) {
    auto interval = make_shared_simple_interval(lower, upper, BorderType::CLOSED, BorderType::CLOSED);
    auto intervals = make_shared_simple_set_set();
    intervals->insert(interval);
    return make_shared_interval(intervals, RealLine::real_line_ptr);
}

inline IntervalPtr_t open(const float lower, const float upper) {
    auto interval = make_shared_simple_interval(lower, upper, BorderType::OPEN, BorderType::OPEN);
    auto intervals = make_shared_simple_set_set();
    intervals->insert(interval);
    return make_shared_interval(intervals, RealLine::real_line_ptr);
}

inline IntervalPtr_t open_closed(const float lower, const float upper) {
    auto interval = make_shared_simple_interval(lower, upper, BorderType::OPEN, BorderType::CLOSED);
    auto intervals = make_shared_simple_set_set();
    intervals->insert(interval);
    return make_shared_interval(intervals, RealLine::real_line_ptr);
}

inline IntervalPtr_t closed_open(const float lower, const float upper) {
    auto interval = make_shared_simple_interval(lower, upper, BorderType::CLOSED, BorderType::OPEN);
    auto intervals = make_shared_simple_set_set();
    intervals->insert(interval);
    return make_shared_interval(intervals, RealLine::real_line_ptr);
}

inline IntervalPtr_t singleton(const float value) {
    auto interval = make_shared_simple_interval(value, value, BorderType::CLOSED, BorderType::CLOSED);
    auto intervals = make_shared_simple_set_set();
    intervals->insert(interval);
    return make_shared_interval(intervals, RealLine::real_line_ptr);
}

inline IntervalPtr_t empty() {
    auto intervals = make_shared_simple_set_set();
    return make_shared_interval(intervals, RealLine::real_line_ptr);
}

inline IntervalPtr_t reals() {
    auto interval = make_shared_simple_interval(-std::numeric_limits<float>::infinity(),
                                                std::numeric_limits<float>::infinity(), BorderType::CLOSED,
                                                BorderType::OPEN);
    auto intervals = make_shared_simple_set_set();
    intervals->insert(interval);
    return make_shared_interval(intervals, RealLine::real_line_ptr);
}
