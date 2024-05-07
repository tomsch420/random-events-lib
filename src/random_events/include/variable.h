#pragma once
#include <string>
#include <memory>
#include "sigma_algebra.h"
#include "interval.h"
#include "set.h"

using NamePtr_t = std::shared_ptr<std::string>;


class AbstractVariable {
public:
    virtual ~AbstractVariable() = default;

    NamePtr_t name;

    virtual AbstractCompositeSetPtr_t get_domain() const = 0;

    bool operator==(const AbstractVariable &other) const {
        return this->name.get() == other.name.get();
    }

    /**
     * Compare two variables. Variables are ordered by their name.
     *
     * Note that the domain is ignored in ordering.
     *
     * @param other The other variable
     * @return True if this variable is less than the other variable.
     */
    bool operator<(const AbstractVariable &other) const {
        return *name < *other.name;
    }

    /**
     * Compare two variables. Variables are ordered by their name.
     *
     * Note that the domain is ignored in ordering.
     *
     * @param other The other variable
     * @return True if this variable is less or equal than the other variable.
     */
    bool operator<=(const AbstractVariable &other) const {
        return *name <= *name;
    }

};

class Symbolic : public AbstractVariable {
public:
    SetPtr_t domain;

    Symbolic(const NamePtr_t name, const SetPtr_t domain) {
        this->name = name;
        this->domain = domain;
    }

    Symbolic(const NamePtr_t name, const AllSetElementsPtr_t all_set_elements) {
        this->name = name;
        auto domain = make_shared_set(all_set_elements);
        for (const auto element: *all_set_elements) {
            auto set_element = make_shared_set_element(element, all_set_elements);
            domain->simple_sets->insert(set_element);
        }
        this->domain = domain;
    }

    AbstractCompositeSetPtr_t get_domain() const override {
        return domain;
    }
};

class Continuous : public AbstractVariable {
public:
    const IntervalPtr_t domain = reals();

    explicit Continuous(const NamePtr_t name) {
        this->name = name;
    }

    AbstractCompositeSetPtr_t get_domain() const override {
        return domain;
    }
};

class Integer : public AbstractVariable {
public:
    const IntervalPtr_t domain = reals();

    explicit Integer(const NamePtr_t name) {
        this->name = name;
    }

    AbstractCompositeSetPtr_t get_domain() const override {
        return domain;
    }
};

using AbstractVariablePtr_t = std::shared_ptr<AbstractVariable>;
using SymbolicPtr_t = std::shared_ptr<Symbolic>;
using IntegerPtr_t = std::shared_ptr<Integer>;
using ContinuousPtr_t = std::shared_ptr<Continuous>;

template<typename... Args>
SymbolicPtr_t make_shared_symbolic(Args &&... args) {
    return std::make_shared<Symbolic>(std::forward<Args>(args)...);
}

template<typename... Args>
IntegerPtr_t make_shared_integer(Args &&... args) {
    return std::make_shared<Integer>(std::forward<Args>(args)...);
}

template<typename... Args>
ContinuousPtr_t make_shared_continuous(Args &&... args) {
    return std::make_shared<Continuous>(std::forward<Args>(args)...);
}

