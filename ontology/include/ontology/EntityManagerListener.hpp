// ----------------------------------------------------------------------------
// EntityManagerListener.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_ENTITY_MANAGER_LISTENER_HPP__
#define __ONTOLOGY_ENTITY_MANAGER_LISTENER_HPP__

// ----------------------------------------------------------------------------
// forward declarations

namespace Ontology {
    class Entity;
}

namespace Ontology {

class EntityManagerListener
{
public:
    virtual void newEntity(Entity*)
    {}
};

} // namespace Ontology

#endif // __ONTOLOGY_ENTITY_MANAGER_LISTENER_HPP__
