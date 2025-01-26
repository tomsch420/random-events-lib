#pragma once

#include "sigma_algebra.h"
#include <set>
#include <utility>

// FORWARD DECLARATIONS
template<typename T>
class SetElement;

template<typename T>
class Set;


// TYPEDEFS
template<typename T>
using AllSetElementsPtr_t = std::shared_ptr<std::set<T>>;
template<typename T>
using SetElementPtr_t = std::shared_ptr<SetElement<T>>;

template<typename T, typename... Args>
AllSetElementsPtr_t<T> make_shared_all_elements(Args &&... args) {
    return std::make_shared<std::set<T>>(std::forward<Args>(args)...);
}


template<typename T, typename... Args>
SetElementPtr_t<T> make_shared_set_element(Args &&... args) {
    return std::make_shared<SetElement<T>>(std::forward<Args>(args)...);
}

template<typename T>
using SetPtr_t = std::shared_ptr<Set<T>>;

template<typename T, typename... Args>
SetPtr_t<T> make_shared_set(Args &&... args) {
    return std::make_shared<Set<T>>(std::forward<Args>(args)...);
}


template<typename T>
class SetElement : public AbstractSimpleSet {
public:

    /**
     * The set of all possible strings
     */
    AllSetElementsPtr_t<T> all_elements;

    /**
     * The index of the element_index in the all_elements set
     */
    int element_index;

    explicit SetElement(const AllSetElementsPtr_t<T> &all_elements_);

    SetElement(const T &element_, const AllSetElementsPtr_t<T> &all_elements_);

    ~SetElement() override;

    AbstractSimpleSetPtr_t intersection_with(const AbstractSimpleSetPtr_t &other) override;

    SimpleSetSetPtr_t complement() override;

    bool contains(const ElementaryVariant *element) override;

    bool is_empty() override;

    /**
     * Two simple sets are equal if the element_index is equal. The all_elements set is not considered.
     *
     * @param other The other simple set.
     * @return True if they are equal.
     */
    bool operator==(const AbstractSimpleSet &other) override;

    bool operator==(const SetElement &other);

    std::string *non_empty_to_string() override;

    bool operator<(const AbstractSimpleSet &other) override;

    /**
     * Compare two set elements. Set elements are ordered by their element index.
     *
     * Note that all elements set is ignored in ordering.
     *
     * @param other The other interval
     * @return True if this interval is less than the other interval.
     */
    bool operator<(const SetElement &other);


    /**
    * Compare two simple intervals. Simple intervals are ordered by lower bound. If the lower bound is equal, they are
    * ordered by upper bound.
    *
    * Note that border types are ignored in ordering.
    *
    * @param other The other interval
    * @return True if this interval is less or equal to the other interval.
    */
    bool operator<=(const SetElement &other);

};


template<typename T>
class Set : public AbstractCompositeSet {
public:

    AllSetElementsPtr_t<T> all_elements;

    explicit Set(const AllSetElementsPtr_t<T>& all_elements_);
    Set(const SetElementPtr_t<T>& element_, const AllSetElementsPtr_t<T>& all_elements_);
    Set(const SimpleSetSetPtr_t& elements, const AllSetElementsPtr_t<T>& all_elements_);

    ~Set() override;

    AbstractCompositeSetPtr_t simplify() override;

    AbstractCompositeSetPtr_t make_new_empty() const override;

    std::string *to_string() override;

};