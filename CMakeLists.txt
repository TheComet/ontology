cmake_minimum_required (VERSION 3.3)

project (ontology
    VERSION 0.0.1
    LANGUAGES CXX)

###############################################################################
# options
###############################################################################

option (ONTOLOGY_ASSERT "Enables assertion. If exceptions are enabled, this option will have no effect" OFF)
option (ONTOLOGY_EXCEPTIONS "Enables exceptions. If assertion is enabled, this will override that option" ON)
option (ONTOLOGY_INSTALL "Whether or not to install files" ON)
option (ONTOLOGY_PIC "Generate position independent code" OFF)
option (ONTOLOGY_SHARED "Whether to build a shared or static version of this library" OFF)
option (ONTOLOGY_TESTS "Whether or not to build ontology's tests" OFF)
option (ONTOLOGY_THREADS "Enables entities to be processed by systems in parallel" OFF)

if (ONTOLOGY_SHARED)
    set (LIB_TYPE "SHARED")
else ()
    set (LIB_TYPE "STATIC")
endif ()

# generate config file
configure_file ("ontology/include/ontology/Config.hpp.in" "ontology/include/ontology/Config.hpp")

###############################################################################
# source and header files
###############################################################################

set (ontology_CONFIG_IN
    "ontology/include/ontology/Config.hpp.in")

set (ontology_HEADERS
    "ontology/include/ontology/Config.hpp"
    "ontology/include/ontology/Component.hpp"
    "ontology/include/ontology/Entity.hpp"
    "ontology/include/ontology/Entity.hxx"
    "ontology/include/ontology/EntityManager.hpp"
    #"ontology/include/ontology/EntityManager.hxx"
    "ontology/include/ontology/EntityManagerListener.hpp"
    "ontology/include/ontology/Exception.hpp"
    "ontology/include/ontology/ListenerDispatcher.hpp"
    "ontology/include/ontology/ListenerDispatcher.hxx"
    "ontology/include/ontology/System.hpp"
    "ontology/include/ontology/System.hxx"
    "ontology/include/ontology/SystemManager.hpp"
    "ontology/include/ontology/SystemManager.hxx"
    "ontology/include/ontology/Type.hpp"
    "ontology/include/ontology/World.hpp")

set (ontology_SOURCES
    "ontology/src/Entity.cpp"
    "ontology/src/EntityManager.cpp"
    "ontology/src/EntityManagerListener.cpp"
    "ontology/src/Exception.cpp"
    "ontology/src/System.cpp"
    "ontology/src/SystemManager.cpp"
    "ontology/src/Type.cpp"
    "ontology/src/World.cpp"
    "ontology/src/allocators/Instancer.cpp"
    "ontology/src/allocators/NullAllocator.cpp"
    "ontology/src/allocators/Mallocator.cpp")

###############################################################################
# create target
###############################################################################

add_library (ontology ${LIB_TYPE}
    ${ontology_CONFIG_IN}
    ${ontology_HEADERS}
    ${ontology_SOURCES})

###############################################################################
# compiler flags
###############################################################################

target_compile_options (ontology
    PUBLIC
        $<$<CXX_COMPILER_ID:GNU>:-pedantic>
        $<$<CXX_COMPILER_ID:Clang>:-pedantic>)

target_compile_features (ontology
    PUBLIC
        cxx_std_17)

target_compile_definitions (ontology
    PRIVATE
        ONTOLOGY_BUILDING)

set_property (TARGET ontology
    PROPERTY POSITION_INDEPENDENT_CODE ${ONTOLOGY_PIC})

target_include_directories (ontology
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ontology/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/ontology/include>
        $<INSTALL_INTERFACE:include>)

###############################################################################
# handle multithreading option - boost is required if enabled
###############################################################################

if (ONTOLOGY_THREAD)
    find_package (Boost REQUIRED COMPONENTS
        system
        thread
    )

    target_link_libraries (ontology
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
    )

    # work around gcc bug on unix where -pthread doesn't link against
    # pthread and -fopenmp doesn't link against libgomp
    if (UNIX)
        target_link_libraries (ontology PRIVATE pthread gomp)
    endif ()
endif ()

###############################################################################
# install targets
###############################################################################

if (ONTOLOGY_INSTALL)
    install (TARGETS ontology DESTINATION "lib")
    install (FILES ${ontology_HEADERS} DESTINATION "include/ontology")
endif ()

if (ONTOLOGY_TESTS)
    add_subdirectory ("tests")
endif ()
