add_library(random_events_lib
        interval.cpp
        set.cpp
        product_algebra.cpp
        sigma_algebra.cpp
)
target_include_directories(random_events_lib PUBLIC include)