#pragma once

#include "sigma_algebra.h"
#include "interval.h"
#include "set.h"
#include <string>
#include <utility>
#include <iostream>
#include <variant>


/**
 * Template class for variables.
 */
template<typename T_Variable, typename T_Domain>
class Variable {
public:

    Variable(std::string name, T_Domain domain): name(std::move(name)), domain(std::move(domain)){};

    /**
     * The name of the variable.
     */
    std::string name;

    /**
     * The domain of the variable.
     * The domain is the set of all possible values.
     */
    const T_Domain domain;

    template <typename T>
    bool operator== (const T &other) const {
        return name == other.name;
    }

    template <typename T>
    bool operator!= (const T &other) const {
        return name != other.name;
    }

    [[nodiscard]] std::string to_string() const {
        return name;
    }

    template <typename T>
    bool operator<(const T &other) const {
        return name < other.name;
    }

    template <typename T>
    bool operator>(const T &other) const {
        return name > other.name;
    }

    template <typename T>
    bool operator<=(const T &other) const {
        return name <= other.name;
    }

    template <typename T>
    bool operator>=(const T &other) const {
        return name >= other.name;
    }

};

/**
 * Class that represents a symbolic variable.
 */
class Symbolic: public Variable<Symbolic, Set>{
public:
    explicit Symbolic(std::string  name, Set domain): Variable<Symbolic, Set>(std::move(name), std::move(domain)){};
};


/**
 * Class that represents an integer variable.
 */
class Integer: public Variable<Integer, Interval>{
public:

    [[maybe_unused]] const Interval domain = reals();

    explicit Integer(std::string name) : Variable(name, reals()) {
        Variable<Integer, Interval>::name = std::move(name);
    };
};

/**
 * Class that represents a continuous variable.
 */
class Continuous: public Variable<Continuous, Interval>{
public:

    [[maybe_unused]] const Interval domain = reals();

    explicit Continuous(std::string name) : Variable(name, reals()) {
        Variable<Continuous, Interval>::name = std::move(name);
    };
};
//
//template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
//std::variant<Continuous, Integer, Symbolic> variable_variant;
//
//struct VisitVariable{
//
//    template<typename T_Variable, typename T_Domain>
//    static std::function<T_Domain(const T_Variable &variable)> get_domain = [] (const T_Variable &variable) {
//        return variable.domain;
//    };
//
//};
//
//std::visit(overload{
//        [](Continuous& v)       { get_domain<Continuous, Interval>(v); },
//        [](Integer& v)          { get_domain<Integer, Interval>(v); },
//        [](Symbolic&v )         { get_domain<Symbolic, Set>(v); },
//
//}, VariableVariant);