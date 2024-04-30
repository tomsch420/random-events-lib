#pragma once

#include "sigma_algebra.h"
#include <map>
#include <memory>
#include "variable.h"
#include <variant>
#include "variable.h"

using VariableVariant = std::variant<Continuous, Integer, Symbolic>;
using SetType = std::variant<Interval, Set>;
using VariableAssignmentType = std::map<VariableVariant, SetType>;

class Event; // Forward declaration

class SimpleEvent : public SimpleSetWrapper<Event, SimpleEvent, std::tuple<>> {
public:

    SimpleEvent() = default;

    explicit SimpleEvent(VariableAssignmentType &variableAssignmentType);

    VariableAssignmentType variable_assignments;

    SimpleEvent simple_set_intersection_with(const SimpleEvent &other) const;

    Event simple_set_complement() const;

    bool simple_set_contains(const std::tuple<> &element) const;

    bool simple_set_is_empty();

    /**
     * Merge the keys of this variable assignment with another variable assignment.
     * @param other_assignments The other variable assignment.
     * @return The merged keys.
     */
    std::set<VariableVariant> merge_keys_of_assignments(const VariableAssignmentType &other_assignments) const;

};

/**
 * Class that represents the product algebra.
 */
class Event : public CompositeSetWrapper<Event, SimpleEvent, std::tuple<int>> {

    Event composite_set_simplify();

};