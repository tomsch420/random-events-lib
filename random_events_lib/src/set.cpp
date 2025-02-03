#include "set.h"
#include <algorithm>
#include <stdexcept>


SetElement::SetElement(const AllSetElementsPtr_t &all_elements_) {
    this->all_elements = all_elements_;
    this->element_index = -1;
}

SetElement::SetElement(int element_index, const AllSetElementsPtr_t &all_elements_) {
    this->all_elements = all_elements_;
    this->element_index = element_index;
    if (element_index < 0) {
        throw std::invalid_argument("element_index must be non-negative");
    }

    if (element_index >= all_elements->size()) {
        throw std::invalid_argument("element_index must be less than the number of elements in the all_elements set");
    }
}

SetElement::~SetElement() = default;

AbstractSimpleSetPtr_t SetElement::intersection_with(const AbstractSimpleSetPtr_t &other) {
    const auto derived_other = (SetElement *)other.get();
    auto result = make_shared_set_element(all_elements);
    if (this->element_index == derived_other->element_index) {
        result->element_index = this->element_index;
    }
    return result;
}

SimpleSetSetPtr_t SetElement::complement() {
    auto result = make_shared_simple_set_set();
    for (int i = 0; i < all_elements->size(); i++) {
        if (i == element_index) {
            continue;
        }
        result->insert(make_shared_set_element(i, all_elements));
    }

    return result;
}

bool SetElement::contains(const ElementaryVariant *element) {
    return false;
}

bool SetElement::is_empty() {
    return this->element_index < 0;
}

bool SetElement::operator==(const AbstractSimpleSet &other) {
    auto derived_other = (SetElement *) &other;
    return *this == *derived_other;
}

bool SetElement::operator==(const SetElement &other) {
    return element_index == other.element_index;
}

bool SetElement::operator<(const AbstractSimpleSet &other) {
    const auto derived_other = (SetElement *) &other;
    return *this < *derived_other;
}

bool SetElement::operator<(const SetElement &other) {
    return element_index < other.element_index;
}

bool SetElement::operator<=(const SetElement &other) {
    return element_index <= other.element_index;
}

std::string *SetElement::non_empty_to_string() {
    return new std::string(std::to_string(element_index));
}

Set::Set(const SetElementPtr_t &element_, const AllSetElementsPtr_t &all_elements_) {
    this->simple_sets = make_shared_simple_set_set();
    this->simple_sets->insert(element_);
    this->all_elements = all_elements_;
}

Set::Set(const AllSetElementsPtr_t &all_elements_) {
    this->simple_sets = make_shared_simple_set_set();
    this->all_elements = all_elements_;
}

Set::Set(const SimpleSetSetPtr_t &elements_, const AllSetElementsPtr_t &all_elements_) {
    this->simple_sets = make_shared_simple_set_set();
    this->simple_sets->insert(elements_->begin(), elements_->end());
    this->all_elements = all_elements_;
}

AbstractCompositeSetPtr_t Set::make_new_empty() const {
    return make_shared_set(all_elements);
}

Set::~Set() {
    simple_sets->clear();
}

AbstractCompositeSetPtr_t Set::simplify() {
    return std::make_shared<Set>(simple_sets, all_elements);
}

std::string *Set::to_string() {
    if (is_empty()) {
        return &EMPTY_SET_SYMBOL;
    }
    auto result = new std::string("{");

    bool first_iteration = true;

    for (const auto &simple_set: *simple_sets) {
        if (first_iteration) {
            first_iteration = false;
        } else {
            result->append(", ");
        }
        result->append(*simple_set->to_string());
    }

    result->append("}");

    return result;
}
