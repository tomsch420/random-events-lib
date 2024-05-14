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

        for (auto const &other_variable: get_variables()) {
            if (processed_variables.find(other_variable) == processed_variables.end()) {
                current_complement->variable_map->insert({other_variable, variable_map->at(other_variable)});
            } else {
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
    auto *result = new std::string("{");

    bool first = true;

    for (auto const &[variable, assignment]: *variable_map) {

        if (first) {
            first = false;
        } else {
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
        if (*assignment != *derived_other->variable_map->at(variable)) {
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


AbstractCompositeSetPtr_t Event::make_new_empty() const {
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
    simple_sets = make_shared_simple_set_set();
    this->all_variables = variables;
}

std::tuple<EventPtr_t, bool> Event::simplify_once() {

    std::vector<AbstractSimpleSetPtr_t> simple_sets_vector = std::vector<AbstractSimpleSetPtr_t>(simple_sets->begin(),
                                                                                                 simple_sets->end());
    EventPtr_t result = make_shared_event(all_variables);
    for (const auto &[first, second]: unique_combinations<AbstractSimpleSetPtr_t>(simple_sets_vector)) {
        auto first_simple_event = std::static_pointer_cast<SimpleEvent>(first);
        auto second_simple_event = std::static_pointer_cast<SimpleEvent>(second);
        auto different_variables = VariableSet();
        for (const auto &variable: *all_variables) {
            auto first_assignment = first_simple_event->variable_map->at(variable).get();
            auto second_assignment = second_simple_event->variable_map->at(variable).get();
            if (*first_assignment != *second_assignment) {
                different_variables.insert(variable);
            }
            // if the pair of simple events mismatches in more than one dimension it cannot be simplified
            if (different_variables.size() > 1) {
                break;
            }
        }
        // if the pair of simple events mismatches in more than one dimension it cannot be simplified
        if (different_variables.size() > 1) {
            continue;
        }

        // if the pair of simple events mismatches in exactly one dimension
        if (different_variables.size() == 1) {

            // get the union the two assignments that are different
            auto different_variable = *different_variables.begin();
            auto first_assignment = first_simple_event->variable_map->at(different_variable);
            auto second_assignment = second_simple_event->variable_map->at(different_variable);
            auto simplified = first_assignment->union_with(second_assignment);

            // create a simpler map with the union of the two assignments
            auto new_variable_map = VariableMap();
            for (const auto &variable: *all_variables) {
                if (variable == different_variable) {
                    new_variable_map.insert({variable, simplified});
                } else {
                    new_variable_map.insert({variable, first_simple_event->variable_map->at(variable)});
                }
            }

            // convert to simple event
            auto new_simple_event = make_shared_simple_event(std::make_shared<VariableMap>(new_variable_map));

            // create new composite event
            result->simple_sets->insert(new_simple_event);
            for (const auto &simple_event: *simple_sets) {
                // skip events that got simplified
                if (simple_event != first and simple_event != second) {
                    result->simple_sets->insert(simple_event);
                }
            }
            return std::make_tuple(result, true);

        }

        // This should never happen and is here for safety reasons
        else {
            throw std::invalid_argument("A composite event should never contain two "
                                        "simple events that are equal. Composite event was: " + *this->to_string());
        }

    }

    // if nothing changed, return a copy
    result->simple_sets = make_shared_simple_set_set(*simple_sets);
    return std::make_tuple(result, false);
}

AbstractCompositeSetPtr_t Event::simplify() {
    auto [simplified, changed] = simplify_once();
    while (changed) {
        auto [new_simplified, new_changed] = simplified->simplify_once();
        simplified = new_simplified;
        changed = new_changed;
    }
    return simplified;
}
