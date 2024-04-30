add_library(random_events_lib interval.cpp
        include/variable.h
        include/set.h
        set.cpp
        variable.cpp
        include/product_algebra.h
)
target_include_directories(random_events_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)