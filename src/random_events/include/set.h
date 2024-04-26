#pragma once

#include "sigma_algebra.h"
#include <set>
#include <utility>
#include <algorithm>

class Set; // Forward declaration

class SimpleSet : public SimpleSetWrapper<Set, SimpleSet, std::string> {
public:

    /**
     * A set that contains all elements that are possible. In Kolmogorov's terms, this is the set of elementary events.
     */
    const std::set<std::string> all_elements;

    /**
     * The element itself as string.
     */
    std::string element;

    SimpleSet() = default;

    explicit SimpleSet(std::string element, std::set<std::string> all_elements) : element(std::move(element)),
                                                                                  all_elements(
                                                                                          std::move(all_elements)) {}

    explicit SimpleSet(std::set<std::string> all_elements) : all_elements(std::move(all_elements)) {}

    [[nodiscard]] SimpleSet simple_set_intersection_with(const SimpleSet &other) const;

    Set simple_set_complement() const;

    bool simple_set_contains(const std::string &other_element) const;

    bool simple_set_is_empty() const;

    bool operator==(const SimpleSet &other) const;

    std::string to_string() const;

    explicit operator std::string() const;

};

/**
 * Hash function for strings.
 */
namespace std {
    template<>
    struct hash<SimpleSet> {
        size_t operator()(const SimpleSet &simple_set) const {
            return hash<std::string>()(simple_set.element);
        }
    };
}

class Set : public CompositeSetWrapper<Set, SimpleSet, std::string> {
public:

    std::set<std::string> all_elements;

    explicit Set(const SimpleSetType<SimpleSet> &simple_sets, std::set<std::string> all_elements) :
            empty_simple_set(SimpleSet(std::move(all_elements))) {
        this->simple_sets = simple_sets;
        this->all_elements = all_elements;
        this->empty_simple_set_ptr = &empty_simple_set;
    }

    explicit Set(std::set<std::string> all_elements) :
            empty_simple_set(SimpleSet(std::move(all_elements))) {
        this->all_elements = all_elements;
        this->empty_simple_set_ptr = &empty_simple_set;
    }

    Set composite_set_simplify();

    SimpleSet empty_simple_set;
};