project (ontology)

set (ontology_HEADERS
	"include/ontology/Component.hpp"
	"include/ontology/Entity.hpp"
	"include/ontology/EntityManager.hpp"
	"include/ontology/ListenerDispatcher.hpp"
	"include/ontology/ListenerDispatcher.hxx"
	"include/ontology/System.hpp"
	"include/ontology/SystemManager.hpp"
	"include/ontology/TypeContainers.hpp"
	"include/ontology/World.hpp"
	"include/ontology/Ontology.hpp"
)

set (ontology_SOURCES
	"src/Entity.cpp"
	"src/EntityManager.cpp"
	"src/System.cpp"
	"src/SystemManager.cpp"
	"src/World.cpp"
)

if (BUILD_STATIC)
	add_library (ontology_static STATIC ${ontology_HEADERS} ${ontology_SOURCES})
	set_target_properties (ontology_static PROPERTIES OUTPUT_NAME ontology_s)
	install (TARGETS ontology_static DESTINATION "lib")
endif (BUILD_STATIC)
if (BUILD_SHARED)
	add_library (ontology_shared SHARED ${ontology_HEADERS} ${ontology_SOURCES})
	set_target_properties (ontology_shared PROPERTIES OUTPUT_NAME ontology)
	install (TARGETS ontology_shared DESTINATION "lib")
endif (BUILD_SHARED)

install (FILES ${ontology_HEADERS} DESTINATION "include/ontology")


