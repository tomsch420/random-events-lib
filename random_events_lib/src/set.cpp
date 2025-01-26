#include "set.h"


template<typename T>
SetElement<T>::SetElement(const AllSetElementsPtr_t<T> &all_elements_) {
    this->all_elements = all_elements_;
    this->element_index = -1;
}

template<typename T>
SetElement<T>::SetElement(const T &element_, const AllSetElementsPtr_t<T> &all_elements_) {
    this->all_elements = all_elements_;
    auto it = std::find(all_elements->begin(), all_elements->end(), element_);
    if (it == all_elements->end()) {
        throw std::invalid_argument("element_index must be in the set");
    }
    this->element_index = std::distance(all_elements->begin(), it);
}

template<typename T>
SetElement<T>::~SetElement() = default;

template<typename T>
AbstractSimpleSetPtr_t SetElement<T>::intersection_with(const AbstractSimpleSetPtr_t &other) {
    const auto derived_other = static_cast<SetElement<T> *>(other.get());
    auto result = make_shared_set_element<T>(all_elements);
    if (this->element_index == derived_other->element_index) {
        result->element_index = this->element_index;
    }
    return result;
}

template<typename T>
SimpleSetSetPtr_t SetElement<T>::complement() {
    auto result = make_shared_simple_set_set();
    for (int i = 0; i < all_elements->size(); i++) {
        if (i == element_index) {
            continue;
        }
        auto it= std::next(all_elements->begin(), i);
        T value_at_index = *it;
        result->insert(make_shared_set_element<T>(value_at_index, all_elements));
    }

    return result;
}

template<typename T>
bool SetElement<T>::contains(const ElementaryVariant *element) {
    return false;
}

template<typename T>
bool SetElement<T>::is_empty() {
    return this->element_index < 0;
}

template<typename T>
bool SetElement<T>::operator==(const AbstractSimpleSet &other) {
    auto derived_other = (SetElement<T> *) &other;
    return *this == *derived_other;
}

template<typename T>
bool SetElement<T>::operator==(const SetElement &other) {
    return element_index == other.element_index;
}

template<typename T>
bool SetElement<T>::operator<(const AbstractSimpleSet &other) {
    const auto derived_other = (SetElement<T> *) &other;
    return *this < *derived_other;
}

template<typename T>
bool SetElement<T>::operator<(const SetElement &other) {
    return element_index < other.element_index;
}

template<typename T>
bool SetElement<T>::operator<=(const SetElement &other) {
    return element_index <= other.element_index;
}

template<typename T>
std::string *SetElement<T>::non_empty_to_string() {
    return new std::string(std::to_string(element_index));
}

template<typename T>
Set<T>::Set(const SetElementPtr_t<T> &element_, const AllSetElementsPtr_t<T> &all_elements_) {
    this->simple_sets = make_shared_simple_set_set();
    this->simple_sets->insert(element_);
    this->all_elements = all_elements_;
}

template<typename T>
Set<T>::Set(const AllSetElementsPtr_t<T> &all_elements_) {
    this->simple_sets = make_shared_simple_set_set();
    this->all_elements = all_elements_;
}

template<typename T>
Set<T>::Set(const SimpleSetSetPtr_t &elements_, const AllSetElementsPtr_t<T> &all_elements_) {
    this->simple_sets = make_shared_simple_set_set();
    this->simple_sets->insert(elements_->begin(), elements_->end());
    this->all_elements = all_elements_;
}

template<typename T>
AbstractCompositeSetPtr_t Set<T>::make_new_empty() const {
    return make_shared_set<T>(all_elements);
}

template<typename T>
Set<T>::~Set() {
    simple_sets->clear();
}

template<typename T>
AbstractCompositeSetPtr_t Set<T>::simplify() {
    return std::make_shared<Set<T>>(simple_sets, all_elements);
}

template<typename T>
std::string *Set<T>::to_string() {
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

// Explicit template instantiation
template class SetElement<int>;
template class SetElement<float>;
template class SetElement<double>;
template class SetElement<std::string>;

template class Set<int>;
template class Set<float>;
template class Set<double>;
template class Set<std::string>;
