cmake_minimum_required (VERSION 2.6)

# If the user specifies -DCMAKE_BUILD_TYPE on the command line, take their
# definition and dump it in the cache along with proper documentation, otherwise
# set MAKE_BUILD_TYPE to Debug prior to calling PROJECT()
if (DEFINED CMAKE_BUILD_TYPE)
        if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
                set (CMAKE_BUILD_TYPE Debug)
        endif ()
        set (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
else()
        set (CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
endif()

###############################################################################
# options
###############################################################################

option (BUILD_SHARED "Whether to build a shared or static version of this library" ON)
option (WITH_MULTITHREADING "Enables entities to be processed by systems in parallel" ON)
option (BUILD_TESTS "Whether or not to build ontology's tests" OFF)

###############################################################################
# output directories
###############################################################################

if (MSVC)
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
else (MSVC)
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif (MSVC)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

###############################################################################
# compiler flags and definitions
###############################################################################

if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions (-D_DEBUG -DDEBUG)
endif ()

if (UNIX)
	add_definitions (-std=c++11 -pedantic)
	if (WITH_MULTITHREADING)
		add_definitions (-pthread)
	endif (WITH_MULTITHREADING)
endif (UNIX)

if (BUILD_SHARED)
	add_definitions (-DONTOLOGY_SHARED)
else ()
	add_definitions (-DONTOLOGY_STATIC)
endif ()

###############################################################################
# set up project
###############################################################################

include_directories ("ontology/include")
add_subdirectory ("ontology")

if (BUILD_TESTS)
	add_subdirectory ("test")
endif (BUILD_TESTS)
