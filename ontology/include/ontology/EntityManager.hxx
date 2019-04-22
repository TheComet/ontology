// ----------------------------------------------------------------------------
// EntityManager.hxx
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// include files

#include "ontology/Config.hpp"
#include "ontology/Entity.hxx"

// ----------------------------------------------------------------------------
// forward declarations

namespace ontology {
    class Component;
    class ComponentFactoryInterface;
    template <class T> class ComponentFactory;
    class EntityManagerListener;
    class World;
}

namespace ontology {

/*!
 * @brief Manages the creation and destruction of entities.
 *
 * This class is used to create and destroy entities. When you create a World,
 * you can access this class with World::getEntityManager().
 *
 * @see Entity
 * @see World
 */
class ONTOLOGY_PUBLIC_API EntityManager
{
public:

    typedef std::unordered_map<Index, Entity> EntityMap;
    typedef std::unordered_map<std::string, Entity> PrototypeMap;

    /*!
     * @brief Construct with world pointer.
     */
    EntityManager(World*);

    /*!
     * @brief Default destructor
     */
    ~EntityManager();

    EntityPrototype& addPrototype(std::string name);
    EntityManager& removePrototype(std::string name);

    /*!
     * @brief Creates a new entity and adds it to the world.
     *
     * @param name The name to give the new entity. Doesn't have to be globally
     * unique.
     * @return Returns a reference to the created entity for chaining purposes.
     * @warning The returned reference may become invalidated if you add
     * another entity. Behind the scenes, entities are stored in contiguous
     * memory for optimisation, and adding an entity could cause a re-allocation.
     *
     * If you want to hold on to the created entity for future use, then call
     * Entity::getID() and store its globally unique identifier. When you need
     * to access the entity again, use EntityManager::getEntity() to get the
     * Entity assigned to that ID.
     */
    Entity& createEntity(std::string prototypeName="");

    /*!
     * @brief Destroys the specified entity.
     * @param entity The entity to destroy.
     */
    EntityManager& destroyEntity(Entity& entity);

    EntityManager& destroyEntity(ID id);

    /*!
     * @brief Destroys all entities. Everything.
     */
    EntityManager& destroyAllEntities();

    /*!
     * @brief Gets a reference to the entity object.
     * @warning The returned reference may become invalidated if you add
     * another entity. Behind the scenes, entities are stored in contiguous
     * memory for optimisation, and adding an entity could cause a re-allocation.
     * @param entityID The identifier of the entity you wish to get the
     * reference of.
     * @return The reference to the entity.
     */
    Entity& getEntity(ID entityID);

    Entity& getEntity(Index entityIdx);

    Index entityIDToIndex(ID entityID);

    typedef std::unordered_map<TypeID, std::unique_ptr<ComponentFactoryInterface>> ComponentFactoryCollection;
    ONTOLOGY_PRIVATE_API ComponentFactoryCollection& getComponentFactories();

    ONTOLOGY_PRIVATE_API std::vector<Entity>& getEntityList();

private:
    std::vector<Entity>                                    entityList_;
    std::unordered_map<ID, Index>                          entityIndexMap_;
    std::unordered_map<std::string, EntityPrototype>       prototypeMap_;
    ComponentFactoryCollection                             componentFactories_;
    World*                                                 world_;
};

} // namespace ontology
