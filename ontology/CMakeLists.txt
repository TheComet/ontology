project (ontologylib)

###############################################################################
# generate build configuration
###############################################################################

if (WITH_MULTITHREADING)
	set (ONTOLOGY_MULTITHREADING 1)
endif ()
if (BUILD_SHARED)
	set (ONTOLOGY_SHARED 1)
else ()
	set (ONTOLOGY_STATIC 1)
endif ()
configure_file (
	"${CMAKE_CURRENT_LIST_DIR}/include/ontology/Config.hpp.in"
	"${CMAKE_CURRENT_LIST_DIR}/include/ontology/Config.hpp"
)

###############################################################################
# source and header files
###############################################################################

set (ontology_HEADERS
	"include/ontology/Config.hpp"
	"include/ontology/Component.hpp"
	"include/ontology/Entity.hpp"
	"include/ontology/Entity.hxx"
	"include/ontology/EntityManager.hpp"
	"include/ontology/EntityManagerInterface.hpp"
	"include/ontology/EntityManagerListener.hpp"
	"include/ontology/Export.hpp"
	"include/ontology/ListenerDispatcher.hpp"
	"include/ontology/ListenerDispatcher.hxx"
	"include/ontology/System.hpp"
	"include/ontology/SystemManager.hpp"
	"include/ontology/SystemManager.hxx"
	"include/ontology/TypeContainers.hpp"
	"include/ontology/World.hpp"
	"include/ontology/Ontology.hpp"
)

set (ontology_SOURCES
	"src/Component.cpp"
	"src/Entity.cpp"
	"src/EntityManager.cpp"
	"src/EntityManagerListener.cpp"
	"src/System.cpp"
	"src/SystemManager.cpp"
	"src/World.cpp"
)

###############################################################################
# create target
###############################################################################

if (BUILD_SHARED)
	add_library (ontology SHARED ${ontology_HEADERS} ${ontology_SOURCES})
else ()
	add_library (ontology STATIC ${ontology_HEADERS} ${ontology_SOURCES})
endif ()

###############################################################################
# handle multithreading option - boost is required if enabled
###############################################################################

if (WITH_MULTITHREADING)
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
		target_link_libraries (ontology pthread gomp)
	endif ()
endif (WITH_MULTITHREADING)

###############################################################################
# install targets
###############################################################################

install (TARGETS ontology DESTINATION "lib")
install (FILES ${ontology_HEADERS} DESTINATION "include/ontology")


