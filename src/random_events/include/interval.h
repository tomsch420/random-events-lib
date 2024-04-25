//
// Created by tom_sch on 23.04.24.
//

#include "SupportsSetOperations.h"

/**
 * Enum for border types of intervals.
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
inline BorderType intersect_borders(BorderType border_1, BorderType border_2){
    return (border_1 == BorderType::OPEN || border_2 == BorderType::OPEN) ? BorderType::OPEN : BorderType::CLOSED;
}

/**
 * Logically complement a border.
 * @param border The borders to complement.
 * @return The complement a border.
 */
inline BorderType invert_border(BorderType border){
    return border == BorderType::OPEN ? BorderType::CLOSED : BorderType::OPEN;
}

class Interval; // Forward declaration

/**
 * Class that represents an atomic interval.
 */
struct AtomicInterval{

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

    [[nodiscard]] AtomicInterval intersection_with(const AtomicInterval &other) const;

    Interval complement();

    [[nodiscard]] bool contains(float element) const;

    [[nodiscard]] bool contains(AtomicInterval &other) const;

    [[nodiscard]] bool is_empty() const;

    bool operator==(const AtomicInterval &other) const;

    Interval difference_with(const AtomicInterval &other) const;

    Interval difference_with(const Interval &other) const;

    [[nodiscard]] std::string  to_string() const;
};

/**
 * Unique Combinations of atomic intervals within a vector.
 * The unique combinations are pairs of atomic intervals which exclude:
 * -  symmetric pairs (A, A)
 * - (A,B) if (B, A) is already visited and.
 *
 * @param elements The vector of atomic intervals.
 * @return The unique combinations of atomic intervals.
 */
std::vector<std::tuple<AtomicInterval, AtomicInterval>> unique_combinations(const std::vector<AtomicInterval> &elements);

/**
 * Unique Combinations of pairs of indices of a vector.
 * The unique combinations which exclude:
 * -  symmetric pairs (A, A)
 * - (A,B) if (B, A) is already visited and.
 *
 * @param size The size of the vector.
 * @return The unique combinations of indices of the elements.
 */
std::vector<std::tuple<size_t , size_t>> unique_index_combinations(size_t size);

/**
 * Intersections of unique combinations of atomic intervals.
 * @param elements The vector of atomic intervals.
 * @return The intersections of unique combinations of atomic intervals.
 */
std::vector<AtomicInterval> intersections_of_unique_combinations(const std::vector<AtomicInterval> &elements);

/**
 * Struct for sorting by lower value.
 */
struct by_lower_ascending {
    bool operator()(const AtomicInterval &a, const AtomicInterval &b) const {
        if (a.lower == b.lower){
            return a.upper < b.upper;
        }
        return a.lower < b.lower;
    }
};

/**
 * Struct for sorting by upper value.
 */
struct by_upper_ascending {
    bool operator()(const AtomicInterval &a, const AtomicInterval &b) const {
        return a.upper < b.upper;
    }
};

template<typename T>
void extend_vector(std::vector<T> &first, const std::vector<T> &second){
    first.reserve(first.size() + distance(second.begin(),second.end()));
    first.insert(first.end(), second.begin(),second.end());
}

/**
 * Class that represents a general interval.
 * An interval is an (automatically simplified) union of atomic intervals.
 */
class Interval : public SupportsSetOperations<Interval>{

public:

    explicit Interval(const std::vector<AtomicInterval> &intervals);

    explicit Interval();

    [[nodiscard]]

    Interval make_disjoint();

    [[nodiscard]] bool is_empty() const;

    [[nodiscard]] std::string  to_string() const;

    [[nodiscard]] bool is_disjoint() const;

    //Interval union_with(AtomicInterval &atomic);

    /**
     * Form the intersection with an atomic interval assuming the this is already a disjoint union of intervals.
     * @param atomic The atomic event to intersect with.
     * @return The intersection.
     */
    Interval intersection_with(AtomicInterval &atomic);

    Interval intersection_with(const Interval &other);

    Interval difference_with(const Interval &other);

    std::tuple<Interval, Interval> split_into_disjoint_and_non_disjoint();

    Interval simplify();

    bool contains(AtomicInterval element) const;
//
//    [[nodiscard]] bool contains(float element) const;
//
//    [[nodiscard]] bool contains(const Interval &other) const;
//
//    [[nodiscard]] bool operator==(const Interval &other) const;

//    Interval union_with(const Interval &other);
//
//    static const std::vector<AtomicInterval> empty_interval_vector;
//
//    static const Interval EmptyInterval;
//
//    SupportsSetOperations* complement() override;

public:
    /**
     * The automatically simplified union of atomic intervals.
     */
    std::vector<AtomicInterval> intervals;
};

inline Interval closed(float lower, float upper){
    return Interval(std::vector<AtomicInterval>{AtomicInterval{lower, upper, BorderType::CLOSED, BorderType::CLOSED}});
}
inline Interval open(float lower, float upper){
    return Interval(std::vector<AtomicInterval>{AtomicInterval{lower, upper, BorderType::OPEN, BorderType::OPEN}});
}
inline Interval open_closed(float lower, float upper){
    return Interval(std::vector<AtomicInterval>{AtomicInterval{lower, upper, BorderType::OPEN, BorderType::CLOSED}});
}
inline Interval closed_open(float lower, float upper){
    return Interval(std::vector<AtomicInterval>{AtomicInterval{lower, upper, BorderType::CLOSED, BorderType::OPEN}});
}

inline Interval singleton(float value){
    return Interval(std::vector<AtomicInterval>{AtomicInterval{value, value, BorderType::CLOSED, BorderType::CLOSED}});
}

inline Interval empty(){
    return Interval();
}