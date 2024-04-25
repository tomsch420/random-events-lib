# adding the Google_Tests_run target
add_executable(RunUnitTest test_interval.cpp)

include_directories(${SRC_DIR}/random_events/include)

# linking Google_Tests_run with DateConverter_lib which will be tested
target_link_libraries(RunUnitTest random_events_lib)

target_link_libraries(RunUnitTest gtest gtest_main)