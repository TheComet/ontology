project (tests)

if (WITH_EXCEPTIONS)
    set (TESTS_WITH_EXCEPTIONS 1)
elseif (WITH_ASSERT AND CMAKE_BUILD_TYPE MATCHES Debug)
    set (TESTS_WITH_ASSERT 1)
endif ()
configure_file (
    "${CMAKE_CURRENT_LIST_DIR}/include/tests/Config.hpp.in"
    "${CMAKE_CURRENT_LIST_DIR}/include/tests/Config.hpp"
)

add_subdirectory ("gmock")
include_directories ("gmock/include")
include_directories ("gmock/gtest/include")
include_directories ("include")

file (GLOB tests_HEADERS "include/tests/*.hpp")
file (GLOB tests_SOURCES "src/*.cpp")
add_executable (tests ${tests_HEADERS} ${tests_SOURCES})
target_link_libraries (tests
    ontology
    gmock
    gmock_main
)
