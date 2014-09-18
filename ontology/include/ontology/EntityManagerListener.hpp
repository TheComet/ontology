// ----------------------------------------------------------------------------
// EntityManagerListener.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_ENTITY_MANAGER_LISTENER_HPP__
#define __ONTOLOGY_ENTITY_MANAGER_LISTENER_HPP__

// ----------------------------------------------------------------------------
// include files

#include <ontology/Export.hpp>

// ----------------------------------------------------------------------------
// forward declarations

namespace Ontology {
    class Component;
    class Entity;
}

namespace Ontology {

class ONTOLOGY_API EntityManagerListener
{
public:
    virtual void onCreateEntity(const Entity*);

    virtual void onDestroyEntity(const Entity*);

    virtual void onAddComponent(const Entity*, const Component*);

    virtual void onRemoveComponent(const Entity*, const Component*);
};

} // namespace Ontology

#endif // __ONTOLOGY_ENTITY_MANAGER_LISTENER_HPP__
