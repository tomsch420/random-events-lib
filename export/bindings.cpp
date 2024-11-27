#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "interval.h"
#include "product_algebra.h"
#include "set.h"

namespace py = pybind11;


PYBIND11_MODULE(random_events, handle) {
    handle.doc()= "A module for handling random events";
    py::class_<AbstractSimpleSet, std::shared_ptr<AbstractSimpleSet>>(handle, "AbstractSimpleSet");
    py::class_<AbstractCompositeSet, std::shared_ptr<AbstractCompositeSet>>(handle, "AbstractCompositeSet");
    py::enum_<BorderType>(handle, "BorderType")
        .value("OPEN", BorderType::OPEN)
        .value("CLOSED", BorderType::CLOSED);
    py::class_<SimpleInterval<>, AbstractSimpleSet, std::shared_ptr<SimpleInterval<>>>(handle, "SimpleInterval")
        .def(py::init<float, float, BorderType, BorderType>())
        .def("intersect_with", &SimpleInterval<>::intersection_with)
        .def("complement", [](SimpleInterval<> &x){return * x.complement();})
        .def("is_empty", &SimpleInterval<>::is_empty)
        .def("__repr__", &SimpleInterval<>::to_string);
    py::class_<Interval<>, AbstractCompositeSet, std::shared_ptr<Interval<>>>(handle, "Interval")
        .def(py::init<>())
        .def(py::init([](SimpleSetSet_t const &x) {
            auto p = std::make_shared<SimpleSetSet_t>(x);
            return Interval<>(p);
        }))
        .def(py::init<const SimpleInterval<> &>())
        .def(py::init([](SimpleSetSet_t const &x) {
            auto p = std::make_shared<SimpleSetSet_t>(x);
            return Interval<>(p);
        }))
        .def("simplify", &Interval<>::simplify)
        .def("make_new_empty", &Interval<>::make_new_empty)
        .def("__repr__", &Interval<>::to_string);
    handle.def("closed", &closed<DefaultOrderable_T>, "Create a closed interval");
    handle.def("open", &open<DefaultOrderable_T>, "Create an open interval");
    handle.def("closed_open", &closed_open<DefaultOrderable_T>, "Create a closed-open interval");
    handle.def("open_closed", &open_closed<DefaultOrderable_T>, "Create an open-closed interval");
    handle.def("singleton", &singleton<DefaultOrderable_T>, "Create a singleton interval");
    handle.def("empty", &empty<DefaultOrderable_T>, "Create an empty interval");
    handle.def("reals", &reals<DefaultOrderable_T>, "Create the real line interval");
    py::class_<SetElement, std::shared_ptr<SetElement>>(handle, "SetElement")
        .def(py::init([](AllSetElements_t const &x) {
            auto const p = std::make_shared<AllSetElements_t>(x);
            return SetElement(p);
        }))
        .def(py::init([](AllSetElements_t const &x, int const &y) {
            auto const p = std::make_shared<AllSetElements_t>(x);
            return SetElement(y, p);
        }))
        .def(py::init([](AllSetElements_t const &x, std::string const &y) {
            auto const p = std::make_shared<AllSetElements_t>(x);
            return SetElement(y, p);
        }))
        .def_property("all_elements", [](SetElement const &x){return * x.all_elements;},
            [](SetElement x, AllSetElements_t const &v){x.all_elements = make_shared_all_elements(v);})
        .def_readwrite("element_index", &SetElement::element_index)
        .def("complement", [](SetElement &x){return * x.complement();})
        .def("intersect_with", &SetElement::intersection_with)
        .def("is_empty", &SetElement::is_empty)
        .def("non_empty_to_string", &SetElement::non_empty_to_string)
        .def("__repr__", &SetElement::to_string);
    py::class_<Set>(handle, "Set")
        .def(py::init([](AllSetElements_t const &x) {
            auto const p = std::make_shared<AllSetElements_t>(x);
            return Set(p);
        }))
        .def(py::init([](SimpleSetSet_t const &x, AllSetElements_t const &y) {
            auto const p = std::make_shared<SimpleSetSet_t>(x);
            auto const q = std::make_shared<AllSetElements_t>(y);
            return Set(p, q);
        }))
        .def(py::init([](SetElement const &x, AllSetElements_t const &y) {
            auto const q = std::make_shared<SetElement>(x);
            auto const p = std::make_shared<AllSetElements_t>(y);
            return Set(q, p);
        }))
        .def_property("all_elements", [](Set const &x){return * x.all_elements;},
            [](Set x, AllSetElements_t const &v){x.all_elements = make_shared_all_elements(v);})
        .def("simplify", &Set::simplify)
        .def("make_new_empty", &Set::make_new_empty)
        .def("__repr__", &Set::to_string);

    py::class_<SimpleEvent, std::shared_ptr<SimpleEvent>>(handle, "SimpleEvent")
        .def(py::init<>())
        .def(py::init([](VariableMap const &x) {
            auto const p = std::make_shared<VariableMap>(x);
            return SimpleEvent(p);
        }))
        .def(py::init<VariableSet>())
        .def("get_variables", &SimpleEvent::get_variables)
        .def("merge_variables", &SimpleEvent::merge_variables)
        .def("intersection_with", &SimpleEvent::intersection_with)
        .def("complement", [](SimpleEvent &x){return * x.complement();})
        .def("__repr__", &SimpleEvent::to_string);
        // .def_property("variable_map", [](SimpleEvent const &x){return * x.variable_map;}, NULL);


}
