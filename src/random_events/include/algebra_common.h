#pragma once

#include <utility>
#include <variant>
#include "sigma_algebra.h"
#include "interval.h"
#include "set.h"


using SetVariant = std::variant<std::monostate, Interval, Set>;

struct VisitSetVariant {
    SetVariant set_variant;

    VisitSetVariant() = default;

    explicit VisitSetVariant(SetVariant set_variant_) : set_variant(std::move(set_variant_)) {};

    bool operator==(const VisitSetVariant &other) const {
        return set_variant == other.set_variant;
    }

//    bool operator<(const VisitSetVariant &other) const {
//        return set_variant < other.variable_variant;
//    }
//
//    bool operator>(const VisitSetVariant &other) const {
//        return set_variant > other.variable_variant;
//    }

    Interval operator()(Interval &v) { return std::get<Interval>(set_variant); }

    Set operator()(Set &v) { return std::get<Set>(set_variant); }


};
