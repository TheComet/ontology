project (test)

add_subdirectory ("gmock")
include_directories ("gmock/include")
include_directories ("gmock/gtest/include")

file (GLOB test_HEADERS "include/test/*.hpp")
file (GLOB test_SOURCES "src/*.cpp")
add_executable (test ${test_HEADERS} ${test_SOURCES})
target_link_libraries (test
	ontology
	gmock_main
)
