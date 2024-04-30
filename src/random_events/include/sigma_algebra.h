#pragma once

#include <set>
#include <vector>
#include <tuple>
#include <memory>

template<typename T>
using SimpleSetType = std::set<T>;

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
    [[nodiscard]] bool is_empty() const {
        return get_simple_set()->simple_set_is_empty();
    }

    /**
     * Form the difference with another simple set.
     *
     * @param other The other simple set.
     * @return The difference as disjoint composite set.
     */
    T_CompositeSet difference_with(const T_SimpleSet &other) const {

        // get the intersection of both atomic simple_sets
        T_SimpleSet intersection = intersection_with(other);

        // if the intersection is empty, return the current atomic interval as interval
        if (intersection.is_empty()) {
            return T_CompositeSet(*get_simple_set());
        }

        // get the complement of the intersection
        T_CompositeSet complement_of_intersection = intersection.complement();

        // initialize the difference vector
        std::set<T_SimpleSet> difference;

        // for every interval in the complement of the intersection
        for (const T_SimpleSet &simple_set: complement_of_intersection.simple_sets) {

            // intersect this with the current complement of the intersection
            T_SimpleSet intersection_with_complement = intersection_with(simple_set);

            // if the intersection with the complement is not empty, append it to the difference vector
            if (!intersection_with_complement.is_empty()) {
                difference.insert(intersection_with_complement);
            }
        }

        return T_CompositeSet(difference);
    }

    virtual bool operator<(const T_SimpleSet &other) const {
        return get_simple_set()->operator<(other);
    }

    virtual bool operator<=(const T_SimpleSet &other) const {
        return get_simple_set()->operator<=(other);
    }

    bool operator>(const T_SimpleSet &other) const {
        return !operator<=(other);
    }

    bool operator>=(const T_SimpleSet &other) const {
        return !operator<(other);
    }


};


/**
* Interface class for composite elements.
* */
template<typename T_CompositeSet, typename T_SimpleSet, typename T_Elementary>
class CompositeSetWrapper {
public:

    /**
     * @return The composite set pointer that implements the interface.
     */
    T_CompositeSet *get_composite_set() const {
        auto *derived = static_cast<const T_CompositeSet *>(this);
        return (T_CompositeSet *) derived;
    }

    /**
     * Empty simple set.
     */
    T_SimpleSet *empty_simple_set_ptr = nullptr;

    /**
     * Default Constructor.
     */
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

    explicit CompositeSetWrapper(const T_CompositeSet &composite_set) : simple_sets(composite_set.simple_sets) {}

    /**
     * @return True if this is empty.
     */
    [[nodiscard]] bool is_empty() const {
        return simple_sets.empty();
    }

    /**
     * @return True if the composite set is disjoint union of simple sets.
     */
    [[nodiscard]] bool is_disjoint() const {
        for (const auto &[first, second]: unique_combinations(simple_sets_as_vector())) {
            if (!first.intersection_with(second).is_empty()) {
                return false;
            }
        }
        return true;
    }

    /**
     * Simplify the composite set into a shorter but equal representation.
     * The size (shortness9 refers to the number of simple sets contained.
     *
     * This method depends on the type of simple set and has to be overloaded.
     *
     * @return The simplified composite set into a shorter but equal representation.
     */
    T_CompositeSet simplify() {
        return get_composite_set()->composite_set_simplify();
    }

    bool operator==(const T_CompositeSet &other) const {
        return simple_sets == other.simple_sets;
    }

    /**
     * @return the simple sets as vector.
     */
    std::vector<T_SimpleSet> simple_sets_as_vector() const {
        return std::vector<T_SimpleSet>(simple_sets.begin(), simple_sets.end());
    }

    /**
     * @return A string representation of this.
     */
    [[nodiscard]] std::string to_string() const {
        if (is_empty()) {
            return "∅";
        }
        std::string result;

        auto simple_sets_vector = simple_sets_as_vector();

        for (size_t i = 0; i < simple_sets_vector.size(); ++i) {
            result.append(simple_sets_vector[i].to_string());
            if (i != simple_sets_vector.size() - 1) {
                result.append(" u ");
            }
        }
        return result;
    }

    /**
     * Split this composite set into disjoint and non-disjoint parts.
     *
     * This method is required for making the composite set disjoint.
     * The partitioning is done by removing every other simple set from every simple set.
     * The purified simple sets are then disjoint by definition and the pairwise intersections are (potentially)
     * not disjoint yet.
     *
     * This method requires:
     *  - the intersection of two simple sets as a simple set
     *  - the difference of a simple set (A) and another simple set (B) that is completely contained in A (B ⊆ A).
     *      The result of that difference has to be a composite set with only one simple set in it.
     *
     * @return A tuple of disjoint and non-disjoint composite sets.
     */
    std::tuple<T_CompositeSet, T_CompositeSet> split_into_disjoint_and_non_disjoint() const {

        // initialize result for disjoint and non-disjoint sets
        T_CompositeSet disjoint;
        T_CompositeSet non_disjoint;

        // for every pair of simple sets
        for (const auto &simple_set_i: simple_sets) {

            // initialize the difference of A_i
            T_SimpleSet difference = simple_set_i;

            // for every other simple set
            for (const auto &simple_set_j: simple_sets) {

                // if the atomic simple_sets are the same, skip
                if (simple_set_i == simple_set_j) {
                    continue;
                }

                // get the intersection of the atomic simple_sets
                auto intersection = simple_set_i.intersection_with(simple_set_j);

                // if the intersection is not empty, append it to the non-disjoint set
                if (!intersection.is_empty()) {
                    non_disjoint.simple_sets.insert(intersection);
                }

                // get the difference of the simple set with the intersection.
                auto difference_with_intersection = difference.difference_with(intersection);

                // if the difference is empty
                if (difference_with_intersection.is_empty()) {

                    // set the difference to simple empty and skip the rest
                    difference = *empty_simple_set_ptr;
                    continue;
                }

                // The difference should only contain 1 simple set since the intersection is completely in simple_set_i.
                difference = *difference.difference_with(intersection).simple_sets.begin();
            }

            // append the simple_set_i without every other simple set to the disjoint set
            disjoint.simple_sets.insert(difference);
        }
        return std::make_tuple(disjoint, non_disjoint);
    }

    /**
     * Create an equal composite set that contains a disjoint union of simple sets.
     *
     * @return The disjoint composite set.
     */
    T_CompositeSet make_disjoint() const {

        // initialize disjoint, non-disjoint and current sets
        T_CompositeSet disjoint;
        T_CompositeSet intersections;
        T_CompositeSet current_disjoint;

        // start with the initial split
        std::tie(disjoint, intersections) = split_into_disjoint_and_non_disjoint();

        // as long the splitting still produces non-disjoint sets
        while (!intersections.is_empty()) {

            // split into disjoint and non-disjoint sets
            std::tie(current_disjoint, intersections) = intersections.split_into_disjoint_and_non_disjoint();

            // extend the result by the disjoint sets
            disjoint.simple_sets.insert(current_disjoint.simple_sets.begin(), current_disjoint.simple_sets.end());
        }

        // simplify and return the disjoint set
        return disjoint.simplify();
    }


    /**
     * Form the intersection with an simple set.
     * The intersection is only disjoint if this is disjoint.
     * @param simple_set The simple event to intersect with.
     * @return The intersection.
     */
    T_CompositeSet intersection_with(const T_SimpleSet &simple_set) const {
        T_CompositeSet result;
        for (const auto &current_simple_set: simple_sets) {
            T_SimpleSet intersection = current_simple_set.intersection_with(simple_set);
            if (!intersection.is_empty()) {
                result.simple_sets.insert(intersection);
            }
        }
        return result;
    }

    /**
     * Form the intersection with another composite set.
     *
     * The intersection is only disjoint if both composite sets are disjoint.
     *
     * @param other The other composite set.
     * @return The intersection as composite set.
     */
    T_CompositeSet intersection_with(const T_CompositeSet &other) const {
        T_CompositeSet result;
        for (const auto &current_simple_set: simple_sets) {
            auto current_result = other.intersection_with(current_simple_set);
            result.simple_sets.insert(current_result.simple_sets.begin(), current_result.simple_sets.end());
        }
        return result;
    }

    /**
     * @return the complement of a composite set as disjoint composite set.
     */
    T_CompositeSet complement() const {
        T_CompositeSet result;
        bool first_iteration = true;
        for (const auto &simple_set: simple_sets) {
            auto simple_set_complement = simple_set.complement();
            if (first_iteration) {
                first_iteration = false;
                result = simple_set_complement;
                continue;
            }
            result = result.intersection_with(simple_set_complement);
        }
        return result.make_disjoint();
    }

    /**
    * Form the union with a simple set.
    *
    * @param other The other simple set.
    * @return The union as disjoint composite set.
    */
    T_CompositeSet union_with(const T_SimpleSet &other) const {
        T_CompositeSet result = T_CompositeSet(simple_sets);
        result.simple_sets.insert(other);
        return result.make_disjoint();
    }

    /**
     * Form the union with another composite set.
     *
     * @param other The other composite set.
     * @return The union as disjoint composite set.
     */
    T_CompositeSet union_with(const T_CompositeSet &other) const {
        T_CompositeSet result = T_CompositeSet(simple_sets);
        result.simple_sets.insert(other.simple_sets.begin(), other.simple_sets.end());
        return result.make_disjoint();
    }

//    std::unique_ptr<AbstractCompositeSet> union_with(const AbstractCompositeSet &other) const override
//    {
//        auto result = std::make_unique<CompositeSetWrapper>(
//                union_with(static_cast<const T_CompositeSet &>(other)));
//        return result;
//    }

    /**
     * Form the difference with a simple set.
     *
     * @param other the simple set
     * @return The difference as disjoint composite set.
     */
    T_CompositeSet difference_with(const T_SimpleSet &other) const {
        T_CompositeSet result;
        for (const auto &simple_set: simple_sets) {
            auto difference = simple_set.difference_with(other);
            result.simple_sets.insert(difference.simple_sets.begin(), difference.simple_sets.end());
        }
        return result.make_disjoint();
    }

    /**
     * Form the difference with another composite set.
     *
     * @param other The other composite set.
     * @return The difference as disjoint composite set.
     */
    T_CompositeSet difference_with(const T_CompositeSet &other) const {
        T_CompositeSet result;

        for (const auto &own_simple_set: simple_sets) {
            T_CompositeSet current_difference;
            bool first_iteration = true;

            for (const auto &other_simple_set: other.simple_sets) {
                auto difference = own_simple_set.difference_with(other_simple_set);
                if (first_iteration) {
                    first_iteration = false;
                    current_difference = difference;
                    continue;
                }
                current_difference = current_difference.intersection_with(difference);
            }
            result.simple_sets.insert(current_difference.simple_sets.begin(), current_difference.simple_sets.end());
        }
        return result.make_disjoint();
    }

//    std::unique_ptr<AbstractCompositeSet> difference_with(const AbstractCompositeSet &other) const override {
//        auto result = std::make_unique<CompositeSetWrapper>(
//                difference_with(static_cast<const T_CompositeSet &>(other)));
//        return result;
//    }

    /**
     * Check if this contains an elementary object.
     * @param element The element to check.
     * @return True if the element is contained in the composite set.
     */
    bool contains(T_Elementary element) const {
        for (const auto &simple_set: simple_sets) {
            if (simple_set.contains(element)) {
                return true;
            }
        }
        return false;
    }

    bool contains(const T_CompositeSet &other) const {
        return intersection_with(other) == other;
    }

public:
    SimpleSetType<T_SimpleSet> simple_sets;
};


/**
 * Unique Combinations of elements within a vector.
 * The unique combinations are pairs of elements which exclude:
 * - symmetric pairs (A, A)
 * - (A,B) if (B, A) is already visited.
 *
 * @param elements The vector.
 * @return The unique combinations of elements of the vector.
 */
template<typename T>
std::vector<std::tuple<T, T>> unique_combinations(const std::vector<T> &elements) {

    // initialize result
    std::vector<std::tuple<T, T>> combinations;

    // for every pair of elements
    for (std::size_t i = 0; i < elements.size(); ++i) {

        // get element from first vector
        T current_element1 = elements[i];
        for (std::size_t j = 0; j < i; ++j) {
            T current_element2 = elements[j];
            std::tuple<T, T> combination = std::make_tuple(current_element1, current_element2);
            combinations.push_back(combination);
        }
    }
    return combinations;
}

