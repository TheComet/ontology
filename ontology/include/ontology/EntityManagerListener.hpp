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

/*!
 * @brief Listener interface for classes that want to listen to EntityManager events.
 */
class ONTOLOGY_API EntityManagerListener
{
public:

    /*!
     * @brief Called when a new entity was created.
     * @param entity The new entity.
     */
    virtual void onCreateEntity(const Entity* entity);

    /*!
     * @brief Called when an entity is destroyed.
     * @param entity The entity being destroyed.
     */
    virtual void onDestroyEntity(const Entity* entity);

    /*!
     * @brief Called when an entity adds a new component.
     * @param entity The entity adding a new component.
     * @param component The new component being added.
     */
    virtual void onAddComponent(const Entity* entity, const Component* component);

    /*!
     * @brief Called when an entity removes a component.
     * @param entity The entity removing the component.
     * @param component The component being removed.
     */
    virtual void onRemoveComponent(const Entity* entity, const Component* component);
};

} // namespace Ontology

#endif // __ONTOLOGY_ENTITY_MANAGER_LISTENER_HPP__
