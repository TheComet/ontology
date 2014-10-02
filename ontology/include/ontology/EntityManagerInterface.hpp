// ----------------------------------------------------------------------------
// EntityManagerInterface.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_ENTITY_MANAGER_INTERFACE_HPP__
#define __ONTOLOGY_ENTITY_MANAGER_INTERFACE_HPP__

namespace Ontology {

class Component;
class Entity;

struct EntityManagerInterface
{
    virtual ~EntityManagerInterface() {}
    virtual Entity& createEntity(const char* name="") = 0;
    virtual void destroyEntity(Entity& entity) = 0;
    virtual void destroyEntities(const char* name) = 0;
    virtual void informAddComponent(Entity& entity, const Component* component) const = 0;
    virtual void informRemoveComponent(Entity& entity, const Component* component) const = 0;
};

} // namespace Ontology

#endif // __ONTOLOGY_ENTITY_MANAGER_INTERFACE_HPP__
