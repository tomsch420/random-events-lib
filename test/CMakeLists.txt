add_subdirectory(lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target
add_executable(RunUnitTest test_interval.cpp
        test_set.cpp
        test_variable.cpp
        test_product_algebra.cpp)

include_directories(${SRC_DIR}/random_events/include)

# linking Google_Tests_run with random_events_lib which will be tested
target_link_libraries(RunUnitTest random_events_lib)

target_link_libraries(RunUnitTest gtest gtest_main)
