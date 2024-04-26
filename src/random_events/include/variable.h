#pragma once

#include "sigma_algebra.h"
#include "interval.h"
#include <string>
#include <utility>

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

class Continuous: public Variable<Continuous, Interval>{
public:

    [[maybe_unused]] const Interval domain = reals();

    explicit Continuous(std::string name) : Variable(name, reals()) {
        Variable<Continuous, Interval>::name = std::move(name);
    };
};

