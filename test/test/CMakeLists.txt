set (test_SOURCES
	"src/gtest_main.cc"
	"src/test_SystemManager.cpp"
)

if (BUILD_SHARED)
	add_executable (test_shared ${test_SOURCES})
	target_link_libraries (test_shared gtest ontology_shared)
endif (BUILD_SHARED)

if (BUILD_STATIC)
	add_executable (test_static ${test_SOURCES})
	target_link_libraries (test_static gtest ontology_static)
endif (BUILD_STATIC)
