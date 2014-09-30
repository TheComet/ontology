project (ontology)

set (ontology_HEADERS
	"include/ontology/Config.hpp"
	"include/ontology/Component.hpp"
	"include/ontology/Entity.hpp"
	"include/ontology/Entity.hxx"
	"include/ontology/EntityManager.hpp"
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

# handle multithreading option
if (WITH_MULTITHREADING)
	add_definitions (-DONTOLOGY_MULTITHREADING)
	find_package (Boost REQUIRED COMPONENTS
		system
		thread
	)
endif (WITH_MULTITHREADING)

if (BUILD_SHARED)
	add_library (ontology SHARED ${ontology_HEADERS} ${ontology_SOURCES})
else ()
	add_library (ontology STATIC ${ontology_HEADERS} ${ontology_SOURCES})
endif ()

if (WITH_MULTITHREADING)
	target_link_libraries (ontology
		${Boost_SYSTEM_LIBRARY}
		${Boost_THREAD_LIBRARY}
	)

	# work around gcc bug on unix where -pthread doesn't link against
	# pthread
	if (UNIX)
		target_link_libraries (ontology pthread)
	endif ()
endif (WITH_MULTITHREADING)

install (TARGETS ontology DESTINATION "lib")
install (FILES ${ontology_HEADERS} DESTINATION "include/ontology")


