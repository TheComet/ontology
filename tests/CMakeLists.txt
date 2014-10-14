project (tests)

add_subdirectory ("gmock")
include_directories ("gmock/include")
include_directories ("gmock/gtest/include")

file (GLOB tests_HEADERS "include/test/*.hpp")
file (GLOB tests_SOURCES "src/*.cpp")
add_executable (tests ${tests_HEADERS} ${tests_SOURCES})
target_link_libraries (tests
	ontology
	gmock
	gmock_main
)
