#pragma once

#include "sigma_algebra.h"
#include <set>
#include <utility>
#include <algorithm>

// FORWARD DECLARATIONS
class SetElement;

class Set;


// TYPEDEFS
using AllSetElementsPtr_t = std::shared_ptr<int>;
using SetElementPtr_t = std::shared_ptr<SetElement>;
using SetPtr_t = std::shared_ptr<Set>;

template<typename... Args>
AllSetElementsPtr_t make_shared_all_elements(Args &&... args) {
    return std::make_shared<int>(std::forward<Args>(args)...);
}

template<typename... Args>
SetElementPtr_t make_shared_set_element(Args &&... args) {
    return std::make_shared<SetElement>(std::forward<Args>(args)...);
}

template<typename... Args>
SetPtr_t make_shared_set(Args &&... args) {
    return std::make_shared<Set>(std::forward<Args>(args)...);
}


class SetElement : public AbstractSimpleSet {
public:

    /**
     * The element to be chose from the all_elements set
     */
    int element_index;

    /**
     * The length of the set of all elements defined in the python object.
     */
    AllSetElementsPtr_t all_elements_length;



    explicit SetElement(const AllSetElementsPtr_t &all_elements_length);

    SetElement(int element_index, const AllSetElementsPtr_t &all_elements_length);

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
};

class Set : public AbstractCompositeSet {
public:

    AllSetElementsPtr_t all_elements_length;

    Set(){
        this->simple_sets = make_shared_simple_set_set();
    }

    explicit Set(const AllSetElementsPtr_t &all_elements_length);

    Set(const SetElementPtr_t& element_, const AllSetElementsPtr_t &all_elements_length);

    Set(const SimpleSetSetPtr_t& elements, const AllSetElementsPtr_t &all_elements_length);

    ~Set() override;

    AbstractCompositeSetPtr_t simplify() override;

    AbstractCompositeSetPtr_t make_new_empty() const override;

    std::string *to_string() override;

};
