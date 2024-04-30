#include "product_algebra.h"
#include "variable.h"

SimpleEvent SimpleEvent::simple_set_intersection_with(const SimpleEvent &other) const {
    auto result = SimpleEvent();

    auto all_variables = merge_keys_of_assignments(other.variable_assignments);

    for (const auto& variable : all_variables){
        ;
        auto this_assignment = variable_assignments.find(variable);
        auto other_assignment = other.variable_assignments.find(variable);

    }
}

std::set<VisitVariableVariant> SimpleEvent::merge_keys_of_assignments(const VariableAssignmentType &other_assignments) const {
    auto all_variables = std::set<VisitVariableVariant>{};

    for(const auto& pair : variable_assignments) {
        all_variables.insert(pair.first);
    }

    for(const auto& pair : other_assignments) {
        all_variables.insert(pair.first);
    }

    return all_variables;
}

SimpleEvent::SimpleEvent(VariableAssignmentType &variableAssignmentType) {
    variable_assignments = variableAssignmentType;

}

SimpleEvent::SimpleEvent(std::map<VariableVariant, SetType> &assignment) {
    for (const auto& pair : assignment){
        variable_assignments.insert(pair);
    }
}


