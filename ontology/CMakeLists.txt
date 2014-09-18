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

if (BUILD_SHARED)
        add_definitions (-DONTOLOGY_SHARED)
else ()
        add_definitions (-DONTOLOGY_STATIC)
endif ()

if (WITH_MULTITHREADING)
	add_definitions (-DONTOLOGY_MULTITHREADING)
endif (WITH_MULTITHREADING)

if (BUILD_STATIC)
	add_library (ontology_static STATIC ${ontology_HEADERS} ${ontology_SOURCES})
	if (WITH_MULTITHREADING AND UNIX)
		target_link_libraries (ontology_static pthread)
	endif (WITH_MULTITHREADING AND UNIX)
	set_target_properties (ontology_static PROPERTIES OUTPUT_NAME ontology_s)
	install (TARGETS ontology_static DESTINATION "lib")
endif (BUILD_STATIC)
if (BUILD_SHARED)
	add_library (ontology_shared SHARED ${ontology_HEADERS} ${ontology_SOURCES})
	if (WITH_MULTITHREADING AND UNIX)
		target_link_libraries (ontology_shared pthread)
	endif (WITH_MULTITHREADING AND UNIX)
	set_target_properties (ontology_shared PROPERTIES OUTPUT_NAME ontology)
	install (TARGETS ontology_shared DESTINATION "lib")
endif (BUILD_SHARED)

install (FILES ${ontology_HEADERS} DESTINATION "include/ontology")


