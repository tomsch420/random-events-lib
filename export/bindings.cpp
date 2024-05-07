#include "pybind11/pybind11.h"
#include "interval.h"

namespace py = pybind11;

PYBIND11_MODULE(random_events, handle) {
    handle.doc()= "A module for handling random events";
    py::class_<Interval, std::shared_ptr<Interval>>(handle, "Interval")
            .def(py::init<>())
            .def(py::init<const SimpleSetSetPtr_t &>())
            .def(py::init<const SimpleInterval &>())
            .def(py::init<SimpleSetSetPtr_t &>())
            .def("simplify", &Interval::simplify)
            .def("make_new_empty", &Interval::make_new_empty)
            .def("__repr__", &Interval::to_string);
    handle.def("closed", &closed, "Create a closed interval");
    handle.def("open", &open, "Create an open interval");
    handle.def("closed_open", &closed_open, "Create a closed-open interval");
    handle.def("open_closed", &open_closed, "Create an open-closed interval");
    handle.def("singleton", &singleton, "Create a singleton interval");
    handle.def("empty", &empty, "Create an empty interval");
    handle.def("reals", &reals, "Create the real line interval");
}
