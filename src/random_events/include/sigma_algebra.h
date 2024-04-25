#include <unordered_set>
#include <vector>

/**
* Interface class for simple sets.
*/
template<typename T_CompositeSet, typename T_SimpleSet, typename T_Elementary>
class SimpleSetWrapper {

    /**
     * @return The simple set pointer that implements the interface.
     */
    T_SimpleSet *get_simple_set() const {
        auto *derived = static_cast<const T_SimpleSet *>(this);
        return derived;
    }

    /**
    * Intersect this with another simple set.
    *
    * This method depends on the type of simple set and has to be overwritten.
    *
    * @param other the other simples set.
    * @return The intersection of both as simple set.
    */
    [[nodiscard]] T_SimpleSet t_intersection_with(const T_SimpleSet &other) const {
        return get_simple_set()->intersection_with(other);
    }

    /**
    * This method depends on the type of simple set and has to be overwritten.
    *
    * @return The complement of this simple set as disjoint composite set.
    */
    T_CompositeSet t_complement() const {
        return get_simple_set()->complement();
    }

    /**
    * This method depends on the type of simple set and has to be overwritten.
    *
    * @param other The other simple set.
    * @return True if they are equal.
    */
    bool operator==(const T_SimpleSet &other) const {
        return *get_simple_set() == other;
    }

    /**
    * Check if an elementary event is contained in this.
    *
    * This method depends on the type of simple set and has to be overwritten.
    *
    * @param element The element to check.
    * @return True if the element is contained in this.
    */
    [[nodiscard]] bool t_contains(T_Elementary element) const {
        return get_simple_set()->contains(element);
    }

    /**
     * This method depends on the type of simple set and has to be overwritten.
     *
     * @return True if this is empty.
     */
    [[nodiscard]] bool t_is_empty() const{
        return get_simple_set()->is_empty();
    }

    /**
     * Form the difference with another simple set.
     * @param other The other simple set.
     * @return The difference as disjoint composite set.
     */
    [[nodiscard]] T_CompositeSet difference_with(const T_SimpleSet &other) const{

        // get the intersection of both atomic simple_sets
        T_SimpleSet intersection = intersection_with(other);

        // if the intersection is empty, return the current atomic interval as interval
        if (intersection.is_empty()) {
            return Interval({*this});
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
    /**
     * @return The composite set pointer that implements the interface.
     */
    T_CompositeSet *get_composite_set() const {
        auto *derived = static_cast<const T_CompositeSet *>(this);
        return derived;
    }

    CompositeSetWrapper(){};

    /**
     * Construct a composite set from a unordered set of simple sets.
     */
    explicit CompositeSetWrapper(const std::vector<T_SimpleSet> &simple_sets) : simple_sets(simple_sets) {}

public:
    std::vector<T_SimpleSet> simple_sets;

};