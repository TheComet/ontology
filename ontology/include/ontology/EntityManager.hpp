// ----------------------------------------------------------------------------
// EntityManager.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_ENTITY_MANAGER_HPP__
#define __ONTOLOGY_ENTITY_MANAGER_HPP__

// ----------------------------------------------------------------------------
// include files

#include <ontology/Export.hpp>
#include <ontology/Entity.hxx>
#include <ontology/EntityManagerInterface.hpp>
#include <ontology/ListenerDispatcher.hpp>

#include <vector>
#include <memory>

// ----------------------------------------------------------------------------
// forward declarations

namespace Ontology {
    class Component;
    class EntityManagerListener;
}

namespace Ontology {

/*!
 * @brief Manages the creation and destruction of entities.
 *
 * This class is used to create and destroy entities. When you create a World,
 * you can access this class with World::getEntityManager().
 *
 * @see Entity
 * @see World
 */
class ONTOLOGY_API EntityManager : public EntityManagerInterface
{
public:

    typedef std::vector< std::unique_ptr<Entity> > EntityList;

    /*!
     * @brief Creates a new entity and adds it to the world.
     *
     * @param name The name to give the new entity.
     * @note Note that you can have multiple entities with the same name.
     * @return Returns a reference to the newly created entity.
     */
    Entity& createEntity(const char* name="") override;

    /*!
     * @brief Destroys an entity by pointer.
     * @param entity The entity to destroy.
     */
    void destroyEntity(Entity* entity) override;

    /*!
     * @brief Destroys all entities sharing the specified name.
     * @param name The name to search for.
     */
    void destroyEntities(const char* name) override;

    /*!
     * @brief Called by entities when they add a new component.
     * @param entity The entity adding a new component.
     * @param component The component being added.
     */
    void informAddComponent(Entity* entity, const Component* component) const override;

    /*!
     * @brief Called by entities when they remove a component.
     * @param entity The entity removing a component.
     * @param component The component being removed.
     */
    void informRemoveComponent(Entity* entity, const Component* component) const override;

    /*!
     * @brief Gets a list of entities of type EntityManager::EntityList.
     */
    const EntityList& getEntityList() const;

    /*!
     * @brief Register as an EntityManagerListener to listen to EntityManager events.
     */
    ListenerDispatcher<EntityManagerListener> event;

private:

    EntityList m_EntityList;
};

} // namespace Ontology

#endif // __ONTOLOGY_ENTITY_MANAGER_HPP__
