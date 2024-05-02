#include "sigma_algebra.h"


SimpleSetSetPtr_t AbstractSimpleSet::difference_with(const AbstractSimpleSetPtr_t &other) const {

    // get the intersection of both atomic simple_sets
    auto intersection = intersection_with(other);

    // if the intersection is empty, return the current atomic interval as interval
    if (intersection->is_empty()) {
        auto result = make_shared_simple_set_set();
        auto po = share_more();
        result->insert();
        return result;
    }

    // get the complement of the intersection
    auto complement_of_intersection = intersection->complement();

    // initialize the difference vector
    auto difference = make_shared_simple_set_set();

    // for every interval in the complement of the intersection
    for (auto const &simple_set: *complement_of_intersection) {

        // intersect this with the current complement of the intersection
        auto intersection_with_complement = intersection_with(simple_set);

        // if the intersection with the complement is not empty, append it to the difference vector
        if (!intersection_with_complement->is_empty()) {
            difference->insert(intersection_with_complement);
        }
    }
    return difference;

}

std::string *AbstractSimpleSet::to_string() const {
    if (is_empty()) {
        return &EMPTY_SET_SYMBOL;
    }
    auto result = new std::string;
    result->append(*non_empty_to_string());
    return result;
}

bool AbstractSimpleSet::operator!=(const AbstractSimpleSet &other) const {
    return !operator==(other);
}

bool AbstractSimpleSet::operator>(const AbstractSimpleSet &other) const {
    return !operator<=(other);
}

bool AbstractSimpleSet::operator>=(const AbstractSimpleSet &other) const {
    return !operator<(other);
}

bool AbstractCompositeSet::is_disjoint() const {
    std::vector<AbstractSimpleSetPtr_t> simple_sets_vector = std::vector<AbstractSimpleSetPtr_t>(simple_sets->begin(),
                                                                                                 simple_sets->end());
    for (const auto &[first, second]: unique_combinations<AbstractSimpleSetPtr_t>(simple_sets_vector)) {
        if (!first->intersection_with(second)->is_empty()) {
            return false;
        }
    }
    return true;
}

bool AbstractCompositeSet::is_empty() const {
    return simple_sets->empty();
}

std::string *AbstractCompositeSet::to_string() const {
    if (is_empty()) {
        return &EMPTY_SET_SYMBOL;
    }
    auto result = new std::string;

    bool first_iteration = true;

    for (const auto &simple_set: *simple_sets) {
        if (first_iteration) {
            first_iteration = false;
        } else {
            result->append(" u ");
        }
        result->append(*simple_set->to_string());
    }
    return result;
}

std::tuple<AbstractCompositeSetPtr_t, AbstractCompositeSetPtr_t>
AbstractCompositeSet::split_into_disjoint_and_non_disjoint() const {

    // initialize result for disjoint and non-disjoint sets
    auto disjoint = make_new_empty(all_elements);
    auto non_disjoint = make_new_empty(all_elements);

    // for every pair of simple sets
    for (const auto &simple_set_i: *simple_sets) {

        // initialize the difference of A_i
        auto difference = simple_set_i;

        // for every other simple set
        for (const auto &simple_set_j: *simple_sets) {

            // if the atomic simple_sets are the same, skip
            if (simple_set_i == simple_set_j) {
                continue;
            }

            // get the intersection of the atomic simple_sets
            auto intersection = simple_set_i->intersection_with(simple_set_j);

            // if the intersection is not empty, append it to the non-disjoint set
            if (!intersection->is_empty()) {
                non_disjoint->simple_sets->insert(intersection);
            }

            // get the difference of the simple set with the intersection.
            auto difference_with_intersection = difference->difference_with(intersection);

            // if the difference is empty
            if (difference_with_intersection->empty()) {

                // set the difference to simple empty and skip the rest
                difference = empty_simple_set_ptr;
                continue;
            }

            // The difference should only contain 1 simple set since the intersection is completely in simple_set_i.
            difference = *difference->difference_with(intersection)->begin();
        }

        // append the simple_set_i without every other simple set to the disjoint set
        disjoint->simple_sets->insert(difference);
    }

    return std::make_tuple(disjoint, non_disjoint);
}

AbstractCompositeSetPtr_t AbstractCompositeSet::make_disjoint() const {

    // initialize disjoint, non-disjoint and current sets
    AbstractCompositeSetPtr_t disjoint;
    AbstractCompositeSetPtr_t intersections;
    AbstractCompositeSetPtr_t current_disjoint;

    // start with the initial split
    std::tie(disjoint, intersections) = split_into_disjoint_and_non_disjoint();

    // as long the splitting still produces non-disjoint sets
    while (!intersections->is_empty()) {

        // split into disjoint and non-disjoint sets
        std::tie(current_disjoint, intersections) = intersections->split_into_disjoint_and_non_disjoint();

        // extend the result by the disjoint sets
        disjoint->simple_sets->insert(current_disjoint->simple_sets->begin(), current_disjoint->simple_sets->end());
    }

    // simplify and return the disjoint set
    return disjoint->simplify();
}

AbstractCompositeSetPtr_t AbstractCompositeSet::intersection_with(const AbstractSimpleSetPtr_t &simple_set) const {
    auto result = make_new_empty(all_elements);
    for (const auto& current_simple_set: *simple_sets) {
        auto intersection = current_simple_set->intersection_with(simple_set);
        if (!intersection->is_empty()) {
            result->simple_sets->insert(intersection);
        }
    }
    return result;
}

AbstractCompositeSetPtr_t AbstractCompositeSet::intersection_with(const SimpleSetSetPtr_t &other) const {
    auto result = make_new_empty(all_elements);
    for (const auto& current_simple_set: *other) {
        auto current_result = intersection_with(current_simple_set);
        result->simple_sets->insert(current_result->simple_sets->begin(), current_result->simple_sets->end());
    }
    return result;
}

AbstractCompositeSetPtr_t AbstractCompositeSet::intersection_with(const AbstractCompositeSetPtr_t &other) const {
    return intersection_with(other->simple_sets);
}

AbstractCompositeSetPtr_t AbstractCompositeSet::complement() const {
    auto result = make_new_empty(all_elements);
    bool first_iteration = true;
    for (const auto& simple_set: *simple_sets) {
        auto simple_set_complement = simple_set->complement();
        if (first_iteration) {
            first_iteration = false;
            result->simple_sets->insert(simple_set_complement->begin(), simple_set_complement->end());
            continue;
        }
        result = result->intersection_with(simple_set_complement);
    }
    return result->make_disjoint();
}

AbstractCompositeSetPtr_t AbstractCompositeSet::union_with(const AbstractSimpleSetPtr_t &other) const {
    auto result = make_new_empty(all_elements);
    result->simple_sets->insert(simple_sets->begin(), simple_sets->end());
    result->simple_sets->insert(other);
    return result->make_disjoint();
}

AbstractCompositeSetPtr_t AbstractCompositeSet::union_with(const AbstractCompositeSetPtr_t &other) const {
    auto result = make_new_empty(all_elements);
    result->simple_sets->insert(simple_sets->begin(), simple_sets->end());
    result->simple_sets->insert(other->simple_sets->begin(), other->simple_sets->end());
    return result->make_disjoint();
}

AbstractCompositeSetPtr_t AbstractCompositeSet::difference_with(const AbstractSimpleSetPtr_t &other) const {
    auto result = make_new_empty(all_elements);
    for (const auto& simple_set: *simple_sets) {
        const auto difference = simple_set->difference_with(other);
        result->simple_sets->insert(difference->begin(), difference->end());
    }
    return result->make_disjoint();
}

AbstractCompositeSetPtr_t AbstractCompositeSet::difference_with(const AbstractCompositeSetPtr_t &other) const {
    auto result = make_new_empty(all_elements);

    for (const auto &own_simple_set: *simple_sets) {
        AbstractCompositeSetPtr_t current_difference = make_new_empty(all_elements);
        bool first_iteration = true;

        for (const auto& other_simple_set: *other->simple_sets) {
            const auto difference_with_simple_set = own_simple_set->difference_with(other_simple_set);

            // handle first iteration
            if (first_iteration) {
                first_iteration = false;
                current_difference->simple_sets->insert(difference_with_simple_set->begin(),
                                                       difference_with_simple_set->end());
                continue;
            }

            auto difference = make_new_empty(all_elements);
            difference->simple_sets->insert(difference_with_simple_set->begin(), difference_with_simple_set->end());
            current_difference = current_difference->intersection_with(difference);
        }
        result->simple_sets->insert(current_difference->simple_sets->begin(), current_difference->simple_sets->end());
    }
    return result->make_disjoint();
}

bool AbstractCompositeSet::contains(const AbstractCompositeSetPtr_t &other) const {
    return intersection_with(other) == other;
}