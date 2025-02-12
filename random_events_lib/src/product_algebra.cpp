#include <stdexcept>
#include "product_algebra.h"

AbstractSimpleSetPtr_t SimpleEvent::intersection_with(const AbstractSimpleSetPtr_t &other) {
    const auto derived_other = (SimpleEvent *) other.get();
    auto variables = derived_other->get_variables();
    auto all_variables = merge_variables(variables);

    auto result = make_shared_simple_event();

    for (auto const &variable: *all_variables) {
        auto own_variable = variable_map->find(variable);
        auto other_variable = derived_other->variable_map->find(variable);

        // if variable is in self and other
        if (other_variable != derived_other->variable_map->end() and
            own_variable != variable_map->end()) {

            // get and intersect assignments
            result->variable_map->insert({variable, variable_map->at(variable)->intersection_with(derived_other->variable_map->at(variable))});
            }
        // if the variable is in self
        else if (own_variable != variable_map->end()) {
            // get and intersect assignments
            result->variable_map->insert({variable, variable_map->at(variable)});
        }
        else
        {
            result->variable_map->insert({variable, derived_other->variable_map->at(variable)});
        }
    }
    return result;
}

void SimpleEvent::fill_missing_variables(const VariableSetPtr_t &variables) const {
    for (auto const &variable: *variables) {
        if (variable_map->find(variable) == variable_map->end()) {
            variable_map->insert({variable, variable->get_domain()});
        }
    }
}

VariableSetPtr_t SimpleEvent::get_variables() const {
    VariableSetPtr_t variables = make_shared_variable_set();
    for (auto const &pair: *variable_map) {
        variables->insert(pair.first);
    }
    return variables;
}

VariableSetPtr_t SimpleEvent::merge_variables(const VariableSetPtr_t &other) const {
    VariableSetPtr_t variables = make_shared_variable_set();
    auto variables_self = get_variables();
    for (auto const &variable: *variables_self) {
        variables->insert(variable);
    }
    for (auto const &variable: *other) {
        variables->insert(variable);
    }
    return variables;
}

SimpleEvent::SimpleEvent(VariableMapPtr_t &variable_map) {
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

        auto variables_self = get_variables();
        for (auto const &other_variable: *variables_self) {
            if (other_variable == variable) {
                continue;
            }
            if (processed_variables.find(other_variable) == processed_variables.end()) {
                current_complement->variable_map->insert({other_variable, other_variable->get_domain()});
            } else {
                current_complement->variable_map->insert({other_variable, variable_map->at(other_variable)});
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
    if (!compare_sets(own_variables, other_variables)) {
        return false;
    }
    for (auto const &[variable, assignment]: *variable_map) {
        auto other_assignment = derived_other->variable_map->at(variable);
        if (*assignment != *other_assignment) {
            return false;
        }
    }
    return true;
}

bool SimpleEvent::operator<(const AbstractSimpleSet &other) {
    auto derived_other = (SimpleEvent *) &other;
    auto own_variables = get_variables();
    auto other_variables = derived_other->get_variables();
    if (own_variables->size() < other_variables->size()) {
        return true;
    }
    for (auto const &[variable, assignment]: *variable_map) {
        if (derived_other->variable_map->find(variable) == derived_other->variable_map->end()) {
            return true;
        }
        if (*assignment == *derived_other->variable_map->at(variable)) {
            continue;
        }
        return *assignment < *derived_other->variable_map->at(variable);
    }
    return false;
}

SimpleEvent::SimpleEvent(const VariableSetPtr_t &variables) {
    variable_map = std::make_shared<VariableMap>();
    for (auto const &variable: *variables) {
        variable_map->insert({variable, variable->get_domain()});
    }
}

SimpleEvent::SimpleEvent() {
    variable_map = std::make_shared<VariableMap>();
}

AbstractSimpleSetPtr_t SimpleEvent::marginal(const VariableSetPtr_t &variables) const {
    auto result = make_shared_simple_event();
    for (auto const &variable: *variables) {
        auto assignment = variable_map->at(variable);
        result->variable_map->insert({variable, assignment});
    }
    return result;
}

Event::Event() {
    simple_sets = make_shared_simple_set_set();
}

Event::Event(const SimpleSetSetPtr_t &simple_events) {
    simple_sets = simple_events;
    fill_missing_variables();
}

Event::Event(const SimpleEventPtr_t &simple_event) {
    simple_sets = make_shared_simple_set_set();
    simple_sets->insert(simple_event);
    fill_missing_variables();
}

void Event::fill_missing_variables(const VariableSetPtr_t &variable_set) const {
    for (auto const &simple_event: *simple_sets) {
        auto casted = (SimpleEvent *) simple_event.get();
        casted->fill_missing_variables(variable_set);
    }
}

void Event::fill_missing_variables() const {
    const auto all_variables = make_shared_variable_set(get_variables_from_simple_events());
    fill_missing_variables(all_variables);
}

AbstractCompositeSetPtr_t Event::make_new_empty() const {
    return make_shared_event();
}

VariableSet Event::get_variables_from_simple_events() const {
    auto result = VariableSet();
    for (auto const &simple_event: *simple_sets) {
        auto casted = (SimpleEvent *) simple_event.get();
        auto variables = casted->get_variables();
        for (auto const &variable: *variables) {
            result.insert(variable);
        }
    }
    return result;
}

std::tuple<EventPtr_t, bool> Event::simplify_once() {

    const auto simple_sets_vector = std::vector<AbstractSimpleSetPtr_t>(simple_sets->begin(), simple_sets->end());
    const auto combinations = unique_combinations<AbstractSimpleSetPtr_t>(simple_sets_vector);
    for (const auto &[first, second]: combinations) {
        const auto event_a = std::static_pointer_cast<SimpleEvent>(first);
        const auto event_b = std::static_pointer_cast<SimpleEvent>(second);
        auto different_variables = VariableSet();
        const auto all_variables = event_a->get_variables();
        for (const auto &variable: *all_variables) {
            const auto first_assignment = event_a->variable_map->at(variable).get();
            const auto second_assignment = event_b->variable_map->at(variable).get();
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
        auto const different_variable = *different_variables.begin();

        auto const simplified_event = make_shared_simple_event();

        for (const auto &variable: *all_variables) {
            if (variable == different_variable) {
                simplified_event->variable_map->insert({variable, event_a->variable_map->at(variable)->union_with(event_b->variable_map->at(variable))});
            } else {
                simplified_event->variable_map->insert({variable, event_a->variable_map->at(variable)});
            }
        }

        // create new composite event
        const EventPtr_t result = make_shared_event();
        result->simple_sets->insert(simplified_event);
        for (const auto &simple_event: *simple_sets) {
            // skip events that got simplified
            if (simple_event != event_a and simple_event != event_b) {
                result->simple_sets->insert(simple_event);
            }
        }
        return std::make_tuple(result, true);
    }

    // if nothing changed, return a copy
    const auto self = make_shared_event();
    self->simple_sets = make_shared_simple_set_set(*simple_sets);
    return std::make_tuple(self, false);
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

AbstractCompositeSetPtr_t Event::marginal(const VariableSetPtr_t& variables) const {
    auto result = make_new_empty();
    for (const auto &simple_set: *simple_sets) {
        auto casted = (SimpleEvent *) simple_set.get();
        result->simple_sets->insert(casted->marginal(variables));
    }
    return result->make_disjoint();
}

