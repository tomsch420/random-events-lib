#pragma once

#include "sigma_algebra.h"
#include "interval.h"
#include <string>

template<typename T_Variable, typename T_Domain>
class Variable {
public:

    /**
     * The name of the variable.
     */
    std::string const name;

    /**
     * The domain of the variable.
     * The domain is the set of all possible values.
     */
    T_Domain const domain;

};

class Continuous: public Variable<Continuous, Interval>{
public:

    Interval const domain = reals();
};

