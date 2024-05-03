#include "set.h"

SetElement::SetElement(int element_, AllSetElementsPtr_t all_elements_) {

    this->element_index = element_;
    this->all_elements = all_elements_;

    if (element_index < 0) {
        throw std::invalid_argument("element_index must be non-negative");
    }

    if (element_index >= all_elements->all_elements_set.size()) {
        throw std::invalid_argument("element_index must be less than the number of elements in the all_elements set");
    }
}

SetElement::SetElement(const std::string &element_, AllSetElementsPtr_t all_elements_) {
    this->all_elements = all_elements_;

    if (element_.empty()) {
        throw std::invalid_argument("element_index must not be empty");
    }

    auto it = std::find(all_elements->all_elements_set.begin(), all_elements->all_elements_set.end(), element_);
    if (it == all_elements->all_elements_set.end()) {
        throw std::invalid_argument("element_index must be in the all_elements set");
    }

    this->element_index = std::distance(all_elements->all_elements_set.begin(), it);

}

SetElement::~SetElement() = default;

AbstractSimpleSetPtr_t SetElement::intersection_with(const AbstractSimpleSetPtr_t &other) {
    const auto derived_other = (SetElement *) other.get();
    auto result = make_shared_set_element(all_elements);
    if (this->element_index == derived_other->element_index) {
        result->element_index = this->element_index;
    }
    return result;
}

SimpleSetSetPtr_t SetElement::complement() {
    auto result = make_shared_simple_set_set();
    for (int i = 0; i < all_elements->all_elements_set.size(); i++) {
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

bool SetElement::operator<=(const AbstractSimpleSet &other) {
    const auto derived_other = (SetElement *) &other;
    return *this <= *derived_other;
}

bool SetElement::operator<=(const SetElement &other) {
    return element_index <= other.element_index;
}

std::string *SetElement::non_empty_to_string() {
    return new std::string(std::to_string(element_index));
}

SetElement::SetElement(AllSetElementsPtr_t all_elements_) {
    this->all_elements = all_elements_;
    this->element_index = -1;
}

AbstractAllElementsPtr_t SetElement::get_all_elements() {
    return all_elements;
}

Set::Set(const SetElementPtr_t& element_, const AllSetElementsPtr_t& all_elements_) {
    this->simple_sets = make_shared_simple_set_set();
    this->simple_sets->insert(element_);
    this->all_elements = all_elements_;
}

Set::Set(const AllSetElementsPtr_t& all_elements_) {
    this->simple_sets = make_shared_simple_set_set();
    this->all_elements = all_elements_;
}

Set::Set(const SimpleSetSetPtr_t& elements_, const AllSetElementsPtr_t& all_elements_) {
    this->simple_sets = elements_;
    this->all_elements = all_elements_;
}

AbstractCompositeSetPtr_t Set::make_new_empty(const AbstractAllElementsPtr_t& all_elements_) {
    return make_shared_set(all_elements_);
}

Set::~Set() {
    simple_sets->clear();
}

AbstractCompositeSetPtr_t Set::simplify()
{
    auto result = make_shared_set(simple_sets, all_elements);
    return result;
}

AbstractCompositeSetPtr_t
Set::make_new(const SimpleSetSetPtr_t& simple_sets_, const AbstractAllElementsPtr_t& all_elements_) {
    AllSetElementsPtr_t casted = std::static_pointer_cast<AllSetElements>(all_elements_);
    return make_shared_set(simple_sets_, casted);
}

AbstractAllElementsPtr_t Set::get_all_elements() {
    return this->all_elements;
}

Set::Set(const AbstractAllElementsPtr_t& all_elements_) {
    this->simple_sets = make_shared_simple_set_set();
    this->all_elements = std::static_pointer_cast<AllSetElements>(all_elements_);
}
