#pragma once

#include "sigma_algebra.h"
#include "interval.h"
#include "set.h"
#include <string>
#include <utility>


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

};


class Symbolic: public Variable<Symbolic, Set>{
public:
    explicit Symbolic(std::string  name, Set domain): Variable<Symbolic, Set>(std::move(name), std::move(domain)){};
};

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

