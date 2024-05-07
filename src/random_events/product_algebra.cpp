#include "product_algebra.h"

AbstractSimpleSetPtr_t SimpleEvent::intersection_with(const AbstractSimpleSetPtr_t &other) {
    const auto derived_other = (SimpleEvent *) other.get();
    auto all_variables = merge_variables(derived_other->get_variables());

    auto result = make_shared_simple_event(all_variables);

    for (auto const &variable: all_variables) {

        // default assignment is the domain of the variable
        auto assignment = result->variable_map->at(variable);

        // if the variable is in self
        if (variable_map->find(variable) != variable_map->end()) {
            // get and intersect assignments
            auto self_assignment = variable_map->at(variable);
            assignment = self_assignment->intersection_with(assignment);
        }

        // if variable is in other
        if (derived_other->variable_map->find(variable) != derived_other->variable_map->end()) {
            // get and intersect assignments
            auto other_assignment = derived_other->variable_map->at(variable);
            assignment = other_assignment->intersection_with(assignment);
        }
        result->variable_map->insert({variable, assignment});
    }
    return result;
}

VariableSet SimpleEvent::get_variables() const {
    VariableSet variables;
    for (auto const &pair: *variable_map) {
        variables.insert(pair.first);
    }
    return variables;
}

VariableSet SimpleEvent::merge_variables(const VariableSet &other) const {
    VariableSet variables;
    for (auto const &variable: get_variables()) {
        variables.insert(variable);
    }
    for (auto const &variable: other) {
        variables.insert(variable);
    }
    return variables;
}

SimpleEvent::SimpleEvent(VariableMapPtr_t variable_map) {
    this->variable_map = variable_map;
}

SimpleSetSetPtr_t SimpleEvent::complement() {

    // initialize result
    auto result = make_shared_simple_set_set();

    // remember which variables got processed already
    auto processed_variables = VariableSet();

    // for every key value pair in the variable map
    for (auto const &[variable, assignment]: *variable_map) {

        // initialize current complement
        auto current_complement = make_shared_simple_event();
        auto complement_of_current_variable = assignment->complement();
        current_complement->variable_map->insert({variable, complement_of_current_variable});

        for (auto const& other_variable: get_variables()) {
            if (processed_variables.find(other_variable) == processed_variables.end()) {
                current_complement->variable_map->insert({other_variable, variable_map->at(other_variable)});
            }
            else
            {
                current_complement->variable_map->insert({other_variable, other_variable->get_domain()});
            }
        }

        // extend processed variables with the current variable
        processed_variables.insert(variable);

        // extend result by current complement if not empty
        if (!current_complement->is_empty()) {
            result->insert(current_complement);
        }
    }

    return result;
}

bool SimpleEvent::contains(const ElementaryVariant *element) {
    return false;
}

bool SimpleEvent::is_empty() {
    if (variable_map->empty()) {
        return true;
    }
    for (auto const &pair: *variable_map) {
        if (pair.second->is_empty()) {
            return true;
        }
    }
    return false;
}

std::string *SimpleEvent::non_empty_to_string() {
    auto* result = new std::string("{");

    bool first = true;

    for (auto const &[variable, assignment]: *variable_map) {

        if (first) {
            first = false;
        }
        else {
            result->append(", ");
        }
        result->append(*variable->name);
        result->append(": ");
        result->append(*assignment->to_string());
    }
    result->append("}");
    return result;
}

bool SimpleEvent::operator==(const AbstractSimpleSet &other) {
    auto derived_other = (SimpleEvent *) &other;
    auto own_variables = get_variables();
    auto other_variables = derived_other->get_variables();
    if (own_variables != other_variables) {
        return false;
    }
    for (auto const &[variable, assignment]: *variable_map) {
        if (assignment != derived_other->variable_map->at(variable)) {
            return false;
        }
    }
    return true;
}

bool SimpleEvent::operator<(const AbstractSimpleSet &other) {
    auto derived_other = (SimpleEvent *) &other;
    for (auto const &[variable, assignment]: *variable_map) {
        if (assignment < derived_other->variable_map->at(variable)) {
            return true;
        }
    }
    return false;
}

bool SimpleEvent::operator<=(const AbstractSimpleSet &other) {
    auto derived_other = (SimpleEvent *) &other;
    for (auto const &[variable, assignment]: *variable_map) {
        if (assignment <= derived_other->variable_map->at(variable)) {
            return true;
        }
    }
    return false;
}

SimpleEvent::SimpleEvent(const VariableSet &variables) {
    variable_map = std::make_shared<VariableMap>();
    for (auto const &variable: variables) {
        variable_map->insert({variable, variable->get_domain()});
    }
}

SimpleEvent::SimpleEvent() {
    variable_map = std::make_shared<VariableMap>();
}

Event::Event() {
    simple_sets = make_shared_simple_set_set();
}

Event::Event(const SimpleSetSetPtr_t &simple_events) {
    simple_sets = simple_events;
    all_variables = make_variable_set(get_variables_from_simple_events());
}

Event::Event(const SimpleEventPtr_t &simple_event) {
    simple_sets = make_shared_simple_set_set();
    simple_sets->insert(simple_event);
    all_variables = make_variable_set(simple_event->get_variables());
}

AbstractCompositeSetPtr_t Event::simplify() {
    auto result = make_shared_event(all_variables);

    // select variables that have different assignment
    auto variables = get_variables_from_simple_events();
    for (auto const &variable: variables) {
        auto assignments = VariableSet();
        for (auto const &simple_event: *simple_sets) {
            auto casted = (SimpleEvent *) simple_event.get();

    }


    // TODO
    return nullptr;
}

AbstractCompositeSetPtr_t Event::make_new_empty() {
    auto result = make_shared_event(all_variables);
    return result;
}

VariableSet Event::get_variables_from_simple_events() const {
    auto result = VariableSet();
    for (auto const &simple_event: *simple_sets) {
        auto casted = (SimpleEvent *) simple_event.get();
        for (auto const &variable: casted->get_variables()) {
            result.insert(variable);
        }
    }
    return result;
}

Event::Event(const VariableSetPtr_t &variables) {
    this->all_variables = variables;
}
