add_library(random_events_lib interval.cpp
        include/variable.h
        include/set.h
        set.cpp
        include/product_algebra.h
        product_algebra.cpp
        include/algebra_common.h
)
target_include_directories(random_events_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)