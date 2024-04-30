#pragma once

#include "sigma_algebra.h"


class Event; // Forward declaration

class SimpleEvent : public SimpleSetWrapper<Event, SimpleEvent, std::tuple<int>> {

};

/**
 * Class that represents the product algebra.
 */
class Event : public CompositeSetWrapper<Event, SimpleEvent, std::tuple<int>> {

};