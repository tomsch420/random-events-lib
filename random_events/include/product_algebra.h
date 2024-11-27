#pragma once

#include "sigma_algebra.h"
#include <map>
#include <memory>
#include "variable.h"
#include <variant>
#include "variable.h"

// FORWARD DECLARATIONS
class SimpleEvent;
class Event;


// TYPEDEFS
using VariableMap = std::map<AbstractVariablePtr_t, AbstractCompositeSetPtr_t>;
using VariableMapPtr_t = std::shared_ptr<VariableMap>;
using SimpleEventPtr_t = std::shared_ptr<SimpleEvent>;
using EventPtr_t = std::shared_ptr<Event>;
using VariableSet = std::set<AbstractVariablePtr_t, PointerLess<AbstractVariablePtr_t>>;
using VariableSetPtr_t = std::shared_ptr<VariableSet>;

template<typename... Args>
SimpleEventPtr_t make_shared_simple_event(Args &&... args) {
    return std::make_shared<SimpleEvent>(std::forward<Args>(args)...);
}

template<typename... Args>
EventPtr_t make_shared_event(Args &&... args) {
    return std::make_shared<Event>(std::forward<Args>(args)...);
}

template<typename... Args>
VariableSetPtr_t make_shared_variable_set(Args &&... args) {
    return std::make_shared<VariableSet>(std::forward<Args>(args)...);
}


class SimpleEvent : public AbstractSimpleSet {
public:
    SimpleEvent();

    explicit SimpleEvent(VariableMapPtr_t &variable_map);

    /**
     * Create a Simple Event where every variable is assigned to its domain.
     * @param variables
     */
    explicit SimpleEvent(const VariableSetPtr_t &variables);

    VariableMapPtr_t variable_map;

    VariableSetPtr_t get_variables() const;

    VariableSetPtr_t merge_variables(const VariableSetPtr_t &other) const;


    AbstractSimpleSetPtr_t intersection_with(const AbstractSimpleSetPtr_t &other) override;

    SimpleSetSetPtr_t complement() override;

    bool contains(const ElementaryVariant *element) override;

    bool is_empty() override;

    std::string *non_empty_to_string() override;

    bool operator==(const AbstractSimpleSet &other) override;

    bool operator<(const AbstractSimpleSet &other) override;

    bool operator<=(const AbstractSimpleSet &other) override;
};

class Event: public AbstractCompositeSet {
public:

    /**
     * The set of all variables used in the events.
     */
    VariableSetPtr_t all_variables;

    Event();
    Event(Event const &event);
    explicit Event(const SimpleSetSetPtr_t &simple_events);
    explicit Event(const SimpleEventPtr_t &simple_event);
    explicit Event(const VariableSetPtr_t &variables);

    VariableSet get_variables_from_simple_events() const;

    AbstractCompositeSetPtr_t simplify() override;
    std::tuple<EventPtr_t , bool> simplify_once();
    AbstractCompositeSetPtr_t make_new_empty() const override;
};