#pragma once

#include "sigma_algebra.h"
#include "set.h"


SimpleSet SimpleSet::simple_set_intersection_with(const SimpleSet &other) const {
    if (element == other.element) {
        return SimpleSet(element, all_elements);
    } else {
        return SimpleSet{all_elements};
    }
}

Set SimpleSet::simple_set_complement() const {
    auto result = Set(all_elements);
    for (auto &element_: all_elements) {
        if (element_ != this->element) {
            result.simple_sets.insert(SimpleSet(element_, all_elements));
        }
    }
    return result;
}

bool SimpleSet::operator==(const SimpleSet &other) const {
    return element == other.element;
}

bool SimpleSet::simple_set_contains(const std::string &other_element) const {
    return element == other_element;
}

std::string SimpleSet::to_string() const {
    return element;
}

SimpleSet::operator std::string() const {
    return to_string();
}

bool SimpleSet::simple_set_is_empty() const {
    return element.empty();
}


Set Set::composite_set_simplify() {
    return *this;
}
