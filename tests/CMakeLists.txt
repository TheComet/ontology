project ("ontology tests")

add_subdirectory ("googletest")

set (tests_HEADERS
    "")
set (tests_SOURCES
    "src/test_allocators_Instancer.cpp"
    "src/test_Entity_AddAndRemoveComponents.cpp"
    "src/test_Entity_ComponentMemoryOrganization.cpp")
add_executable (ontology_tests
    ${tests_HEADERS}
    ${tests_SOURCES})

target_include_directories (ontology_tests
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries (ontology_tests PRIVATE ontology)
target_link_libraries (ontology_tests PRIVATE gmock)
target_link_libraries (ontology_tests PRIVATE gmock_main)
