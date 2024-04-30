# https://decovar.dev/blog/2021/03/08/cmake-cpp-library
# https://medium.com/@kkunalsharma825/install-and-export-in-cmake-5c26d20ff715
# For CMAKE_INSTALL_INCLUDEDIR definition
include(GNUInstallDirs)

add_library(random_events interval.cpp
        include/variable.h
        include/set.h
        include/product_algebra.h
        include/interval.h
        set.cpp
        product_algebra.cpp
        sigma_algebra.cpp
)
target_include_directories(random_events PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(HEADER_FILES 
    include/interval.h 
    include/product_algebra.h
    include/set.h
    include/sigma_algebra.h
    include/variable.h
)
set_target_properties(random_events PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

# Install the main random_events library
install(
        TARGETS random_events
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT runtime
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT dev
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/random_events" COMPONENT dev
)
#OR 
# Install all header files for the main random_events library
#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ #Note the trailing / to signify copying files in the folder only (not the folder itself)
#        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/random_events
#        FILES_MATCHING REGEX ".*\\.(h|inl)$")
