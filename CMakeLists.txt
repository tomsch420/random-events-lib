cmake_minimum_required(VERSION 3.16)
project(random_events)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(src/random_events)

add_subdirectory(test)