// ----------------------------------------------------------------------------
// EntityManager.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_ENTITY_MANAGER_HPP__
#define __ONTOLOGY_ENTITY_MANAGER_HPP__

// ----------------------------------------------------------------------------
// include files

#include <ontology/Entity.hpp>

#include <vector>
#include <memory>

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
class EntityManager
{
public:

    /*!
     * @brief The type of the list containing all of the entities.
     *
     * See EntityManager::getEntityList()
     */
    typedef std::vector< std::unique_ptr<Entity> > EntityList;

    /*!
     * @brief Creates a new entity and adds it to the world.
     *
     * @param name The name to give the new entity.
     * @note Note that you can have multiple entities with the same name.
     * @return Returns a reference to the newly created entity.
     */
    Entity& createEntity(const char* name="");

    /*!
     * @brief Destroys an entity by pointer.
     */
    void destroyEntity(const Entity* entity);

    /*!
     * @brief Destroys all entities sharing the specified name.
     */
    void destroyEntities(const char* name);

    /*!
     * @brief Gets a list of entities of type EntityManager::EntityList.
     */
    const EntityList& getEntityList() const;

private:

    EntityList m_EntityList;
};

} // namespace Ontology

#endif // __ONTOLOGY_ENTITY_MANAGER_HPP__
