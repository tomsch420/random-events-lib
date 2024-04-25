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
struct SimpleInterval {

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
     * Intersect this with another simple set.
     *
     * This method depends on the type of simple set and has to be overloaded.
     *
     * @param other the other simples set.
     * @return The intersection of both as simple set.
     */
    [[nodiscard]] SimpleInterval intersection_with(const SimpleInterval &other) const;

    /**
     * This method depends on the type of simple set and has to be overloaded.
     *
     * @return The complement of this simple set as disjoint composite set.
     */
    Interval complement();

    /**
     * Check if an elementary event is contained in this.
     *
     * This method depends on the type of simple set and has to be overloaded.
     *
     * @param element The element to check.
     * @return True if the element is contained in this.
     */
    [[nodiscard]] bool contains(float element) const;

    /**
     * Check if another simple set is contained in this.
     *
     * @param other The other simple set to check.
     * @return True if the other simple set is contained in this.
     */
    [[nodiscard]] bool contains(SimpleInterval &other) const;

    /**
     * This method depends on the type of simple set and has to be overloaded.
     *
     * @return True if this is empty.
     */
    [[nodiscard]] bool is_empty() const;

    /**
     * This method depends on the type of simple set and has to be overloaded.
     *
     * @param other The other simple set.
     * @return True if they are equal.
     */
    bool operator==(const SimpleInterval &other) const;

    /**
     * Form the difference with another simple set.
     * @param other The other simple set.
     * @return The difference as disjoint composite set.
     */
    [[nodiscard]] Interval difference_with(const SimpleInterval &other) const;


    /**
     * Form the difference with a composite set.
     * @param other The composite set.
     * @return The difference as non-disjoint composite set.
     */
    [[nodiscard]] Interval difference_with(const Interval &other) const;

    // TODO Ask Duc on how to overload the __repr__ function in C++.
    [[nodiscard]] std::string to_string() const;
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
std::vector<std::tuple<SimpleInterval, SimpleInterval>>
unique_combinations(const std::vector<SimpleInterval> &elements);


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
 * An interval is an (automatically simplified) union of simple intervals.
 */
class Interval : public SupportsSetOperations<Interval> {

public:

    /**
     * Construct an interval from a vector of simple sets.
     * @param simple_intervals The vector of simple sets.
     */
    explicit Interval(const std::vector<SimpleInterval> &simple_intervals);

    /**
     * Construct an empty composite set.
     */
    explicit Interval();

    /**
     * Create an equal composite  set that contains a disjoint union of simple sets.
     * @return The disjoint composite set.
     */
    Interval make_disjoint();

    /**
     * Check if the composite set is empty.
     * @return True if the composite set is empty.
     */
    [[nodiscard]] bool is_empty() const;

    //TODO Ask Duc on how to overload the __repr__ function in C++.
    [[nodiscard]] std::string to_string() const;

    /**
     * @return True if the composite set is disjoint union of simple sets.
     */
    [[nodiscard]] bool is_disjoint() const;

    /**
     * Form the intersection with an simple set.
     * The intersection is only disjoint if this is disjoint.
     * @param simple The simple event to intersect with.
     * @return The intersection.
     */
    Interval intersection_with(SimpleInterval &simple);

    /**
     * Form the intersection with another composite set.
     *
     * The intersection is only disjoint if both composite sets are disjoint.
     *
     * @param other The other composite set.
     * @return The intersection as composite set.
     */
    Interval intersection_with(const Interval &other);

    /**
     * Form the difference with another composite set.
     *
     * This difference is not guaranteed disjoint.
     *
     * @param other The other composite set.
     * @return The difference as composite set.
     */
    Interval difference_with(const Interval &other);

    /**
     * Split this composite set into disjoint and non-disjoint parts.
     *
     * This method is required for making the union of composite sets disjoint.
     * The partitioning is done by removing every other simple set from every simple set.
     * The purified simple sets are then disjoint by definition and the pairwise intersections are not disjoint yet.
     *
     * @return A tuple of disjoint and non-disjoint composite sets.
     */
    std::tuple<Interval, Interval> split_into_disjoint_and_non_disjoint();

    /**
     * Simplify the composite set into a shorter but equal representation.
     * The size refers to the number of simple sets contained.
     *
     * * This method depends on the type of simple set and has to be overloaded.
     *
     * @return The simplified composite set into a shorter but equal representation.
     */
    Interval simplify();

    /**
     * Check if a simple set is contained in the composite set.
     * @param element The element to check.
     * @return  True if the element is contained in the composite set.
     */
    [[nodiscard]] bool contains(SimpleInterval element) const;

public:
    /**
     * The automatically simplified union of atomic intervals.
     */
    std::vector<SimpleInterval> intervals;
};

inline Interval closed(float lower, float upper) {
    return Interval(std::vector<SimpleInterval>{SimpleInterval{lower, upper, BorderType::CLOSED, BorderType::CLOSED}});
}

inline Interval open(float lower, float upper) {
    return Interval(std::vector<SimpleInterval>{SimpleInterval{lower, upper, BorderType::OPEN, BorderType::OPEN}});
}

inline Interval open_closed(float lower, float upper) {
    return Interval(std::vector<SimpleInterval>{SimpleInterval{lower, upper, BorderType::OPEN, BorderType::CLOSED}});
}

inline Interval closed_open(float lower, float upper) {
    return Interval(std::vector<SimpleInterval>{SimpleInterval{lower, upper, BorderType::CLOSED, BorderType::OPEN}});
}

inline Interval singleton(float value) {
    return Interval(std::vector<SimpleInterval>{SimpleInterval{value, value, BorderType::CLOSED, BorderType::CLOSED}});
}

inline Interval empty() {
    return Interval();
}

/*
 * The real numbers.
 */
inline Interval reals() {
    return Interval(std::vector<SimpleInterval>{
            SimpleInterval{-std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(),
                           BorderType::OPEN, BorderType::OPEN}});
}