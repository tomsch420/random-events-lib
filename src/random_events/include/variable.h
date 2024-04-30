#pragma once

#include "sigma_algebra.h"
#include "interval.h"
#include "set.h"
#include <string>
#include <utility>
#include <iostream>
#include <variant>


class AbstractVariable {
public:
    /**
     * The (virtual) name of the variable.
     */
    std::string name;

    explicit AbstractVariable(std::string name) : name(std::move(name)){};
};



/**
 * Template class for variables.
 */
template<typename T_Variable, typename T_Domain>
class Variable: public AbstractVariable {
public:
    Variable(std::string name, T_Domain domain) : AbstractVariable(name), domain(std::move(domain)) {};

    /**
     * The domain of the variable.
     * The domain is the set of all possible values.
     */
    const T_Domain domain;

    template<typename T>
    bool operator==(const T &other) const {
        return name == other.name;
    }

    template<typename T>
    bool operator!=(const T &other) const {
        return name != other.name;
    }

    [[nodiscard]] std::string to_string() const {
        return name;
    }

    template<typename T>
    bool operator<(const T &other) const {
        return name < other.name;
    }

    template<typename T>
    bool operator>(const T &other) const {
        return name > other.name;
    }

    template<typename T>
    bool operator<=(const T &other) const {
        return name <= other.name;
    }

    template<typename T>
    bool operator>=(const T &other) const {
        return name >= other.name;
    }

};

/**
 * Class that represents a symbolic variable.
 */
class Symbolic : public Variable<Symbolic, Set> {
public:
    explicit Symbolic(std::string name, Set domain) : Variable<Symbolic, Set>(std::move(name), std::move(domain)) {};
};


/**
 * Class that represents an integer variable.
 */
class Integer : public Variable<Integer, Interval> {
public:

    [[maybe_unused]] const Interval domain = reals();

    explicit Integer(std::string name) : Variable(name, reals()) {
        Variable<Integer, Interval>::name = std::move(name);
    };
};

/**
 * Class that represents a continuous variable.
 */
class Continuous : public Variable<Continuous, Interval> {
public:

    [[maybe_unused]] const Interval domain = reals();

    explicit Continuous(std::string name) : Variable(name, reals()) {
        Variable<Continuous, Interval>::name = std::move(name);
    };

};

using VariableVariant = std::variant<std::monostate, Continuous, Integer, Symbolic>;

struct VisitVariableVariant {
    VariableVariant variable_variant;

    VisitVariableVariant() = default;

    VisitVariableVariant(VariableVariant variable) : variable_variant(std::move(variable)) {};

    bool operator==(const VisitVariableVariant &other) const {
        return variable_variant == other.variable_variant;
    }

    bool operator<(const VisitVariableVariant &other) const {
        return variable_variant < other.variable_variant;
    }

    bool operator>(const VisitVariableVariant &other) const {
        return variable_variant > other.variable_variant;
    }

    Continuous operator()(Continuous &v) { return std::get<Continuous>(variable_variant); }

    Integer operator()(Integer &v) { return std::get<Integer>(variable_variant); }

    Symbolic operator()(Symbolic &v) { return std::get<Symbolic>(variable_variant); }

};
