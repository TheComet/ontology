// ----------------------------------------------------------------------------
// EntityManager.hxx
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// include files

#include "ontology/Config.hpp"
#include "ontology/Entity.hxx"
#include "ontology/ListenerDispatcher.hpp"

#include <unordered_map>
#include <memory>
#include <string>
#include <functional>
#include <cassert>

// ----------------------------------------------------------------------------
// forward declarations

namespace ontology {
    class Component;
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

    /*!
     * @brief Register as an EntityManagerListener to listen to EntityManager events.
     */
    ListenerDispatcher<EntityManagerListener> event;

private:
    friend class Entity;

    class ComponentFactoryInterface
    {
    public:
        ComponentFactoryInterface(EntityManager& entityManager);
        virtual ~ComponentFactoryInterface() {}
        void addRef();
        bool subRef();

    protected:
        EntityManager& entityManager_;

    private:
        virtual Index createComponentForEntityWithDefaultArgs(const Entity& entity) = 0;
        virtual void destroyComponentForEntity(const Entity& e) = 0;
        uint32_t refcount = 1;
    };

    template <class T>
    class ComponentFactory : public ComponentFactoryInterface
    {
    public:
        ComponentFactory(EntityManager& entityManager);

        template <class... Args>
        Index createComponentForEntityWithCustomArgs(const Entity& entity, Args&&... args);
        Index createComponentForEntityWithDefaultArgs(const Entity& entity) override;
        void destroyComponentForEntity(const Entity& e) override;

        T& getComponentForEntity(const Entity& e);
        T& getComponent(Index componentIndex);

    protected:
        std::vector<T> components_;
    };

    template <class T, class... DefaultArgs>
    class DefaultComponentFactory : public ComponentFactory<T>
    {
    public:
        DefaultComponentFactory(EntityManager& entityManager, DefaultArgs&&... args);
        Index createComponentForEntityWithDefaultArgs(const Entity& entity) override final;

    private:
        std::tuple<DefaultArgs...> defaultArgs_;
    };

    template <class T, class... DefaultArgs>
    ONTOLOGY_PRIVATE_API void registerComponentFactory(DefaultArgs&&...);

    template <class T>
    ONTOLOGY_PRIVATE_API void unregisterComponentFactory();
    ONTOLOGY_PRIVATE_API void unregisterComponentFactory(const TypeID&);

    template <class T, class... Args>
    ONTOLOGY_PRIVATE_API Index createComponentForEntity(const Entity& entity, Args&&...);

    template <class T>
    ONTOLOGY_PRIVATE_API void destroyComponentForEntity(const Entity& entity);

    template <class T>
    ONTOLOGY_PRIVATE_API T& getComponentForEntity(const Entity& entity);

    ONTOLOGY_PRIVATE_API std::vector<Entity>& getEntityList();

public: // public due to unit tests
    template <class T>
    ONTOLOGY_PRIVATE_API ComponentFactory<T>* findComponentFactory() const;

private:
    std::vector<Entity>                                    entityList_;
    std::unordered_map<ID, Index>                          entityIndexMap_;
    std::unordered_map<std::string, EntityPrototype>       prototypeMap_;
    std::unordered_map<TypeID, std::unique_ptr<ComponentFactoryInterface>>  componentFactories_;
    World*                                                 world_;
};

} // namespace ontology
