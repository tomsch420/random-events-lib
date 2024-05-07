cmake_minimum_required(VERSION 3.16)
project(random_events)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(src/random_events)
add_subdirectory(test)
add_subdirectory(lib/pybind11)
add_subdirectory(export)