#pragma once
#include <unordered_set>
#include <vector>


template<typename T>
using SimpleSetType = std::unordered_set<T>;

/**
* Interface class for simple sets.
*/
template<typename T_CompositeSet, typename T_SimpleSet, typename T_Elementary>
class SimpleSetWrapper {
public:
    /**
     * @return The simple set pointer that implements the interface.
     */
    T_SimpleSet *get_simple_set() const {
        auto *derived = static_cast<const T_SimpleSet *>(this);
        return (T_SimpleSet *) derived;
    }

    /**
    * Intersect this with another simple set.
    *
    * This method depends on the type of simple set and has to be overwritten.
    *
    * @param other the other simples set.
    * @return The intersection of both as simple set.
    */
    [[nodiscard]] T_SimpleSet intersection_with(const T_SimpleSet &other) const {
        return get_simple_set()->simple_set_intersection_with(other);
    }

    /**
    * This method depends on the type of simple set and has to be overwritten.
    *
    * @return The complement of this simple set as disjoint composite set.
    */
    T_CompositeSet complement() const {
        return get_simple_set()->simple_set_complement();
    }

//    /**
//    * This method depends on the type of simple set and has to be overwritten.
//    *
//    * @param other The other simple set.
//    * @return True if they are equal.
//    */
//    virtual bool operator==(const T_SimpleSet &other) const {
//        return *get_simple_set() == other;
//    }

    /**
    * Check if an elementary event is contained in this.
    *
    * This method depends on the type of simple set and has to be overwritten.
    *
    * @param element The element to check.
    * @return True if the element is contained in this.
    */
    [[nodiscard]] bool contains(const T_Elementary &element) const {
        return get_simple_set()->simple_set_contains(element);
    }

    /**
     * Check if another simple set is contained in this.
     *
     * @param other The other simple set to check.
     * @return True if the other simple set is contained in this.
     */
    bool contains(const T_SimpleSet &other) const {
        return !intersection_with(other).is_empty();
    }

    /**
     * This method depends on the type of simple set and has to be overwritten.
     *
     * @return True if this is empty.
     */
    [[nodiscard]] bool t_is_empty() const {
        return get_simple_set()->is_empty();
    }

    /**
     * Form the difference with another simple set.
     * @param other The other simple set.
     * @return The difference as disjoint composite set.
     */
    [[nodiscard]] T_CompositeSet difference_with(const T_SimpleSet &other) const {

        // get the intersection of both atomic simple_sets
        T_SimpleSet intersection = intersection_with(other);

        // if the intersection is empty, return the current atomic interval as interval
        if (intersection.is_empty()) {
            return T_CompositeSet({*this});
        }

        // get the complement of the intersection
        T_CompositeSet complement_of_intersection = intersection.complement();

        // initialize the difference vector
        std::unordered_set<T_SimpleSet> difference;

        // for every interval in the complement of the intersection
        for (auto interval: complement_of_intersection.simple_sets) {

            // intersect this with the current complement of the intersection
            T_SimpleSet intersection_with_complement = intersection_with(interval);

            // if the intersection with the complement is not empty, append it to the difference vector
            if (!intersection_with_complement.is_empty()) {
                difference.push_back(intersection_with_complement);
            }
        }

        return T_CompositeSet(difference);
    }
};


/**
* Interface class for composite elements.
* */
template<typename T_CompositeSet, typename T_SimpleSet, typename T_Elementary>
class CompositeSetWrapper {
public:

//    struct T_SimpleSetHash {
//        std::size_t operator()(const T_SimpleSet &simple_set) const {
//            return std::hash<T_SimpleSet>{}(simple_set);
//        }
//    };

    /**
     * @return The composite set pointer that implements the interface.
     */
    T_CompositeSet *get_composite_set() const {
        auto *derived = static_cast<const T_CompositeSet *>(this);
        return derived;
    }

    CompositeSetWrapper() {
        simple_sets = SimpleSetType<T_SimpleSet>();
    };

    /**
     * Construct a composite set from a unordered set of simple sets.
     */
    explicit CompositeSetWrapper(const SimpleSetType<T_SimpleSet> &simple_sets_) {
        for (auto simple_set: simple_sets) {
            if (!simple_set.is_empty()) {
                simple_sets.push_back(simple_set);
            }
        }
    }

public:
    SimpleSetType<T_SimpleSet> simple_sets;
};