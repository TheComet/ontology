//----------------------------------------------------------------------------
//Entity.hxx
//----------------------------------------------------------------------------

#pragma once

//----------------------------------------------------------------------------
//include files

#include <ontology/Config.hpp>
#include <ontology/Type.hpp>

#include <map>
#include <typeinfo>
#include <string>
#include <cassert>

//----------------------------------------------------------------------------
//forward declarations

namespace ontology {
    template <class T> class ComponentFactory;
    class ComponentFactoryInterface;
    class EntityManager;
    class System;
}

namespace ontology {

/*!
 * @brief An entity is used to bind components together.
 *
 * Entities can be created through the world and are primarily used to simply
 * contain Components. You can add or remove components from an entity by using
 * its methods Entity::addComponent(new MyComponent) and
 * Entity::removeComponent<Component>(). You can retrieve and modify a
 * component's data at any time with Entity::getComponent<Component>().
 *
 * @note
 * Please note that all methods support chaining, and that the recommended
 * creation of entities is through chaining. The following is an example
 * demonstrating this:
 * @code
 * world.getEntityManager().createEntity("My Entity")
 *     .addComponent<Position>(0, 0)
 *     .addComponent<Velocity>(1, 2)
 *     .addComponent<Sprite>("player.png")
 *     ;
 * @endcode
 * @see Component
 */
class Entity
{
    /// Required for move constructor
    Entity();

public:
    /*!
     * @brief Construct an entity with a name.
     */
    Entity(ID id, EntityManager* entityManager);

    /*!
     * @brief Allow destruction through base class pointer.
     */
    virtual ~Entity();

    /// Don't allow copying. Each entity must have a unique ID
    Entity(const Entity& other) = delete;

    /// Enable move constructing and move assigning so std::vector works
    Entity(Entity&& other);

    Entity& operator=(Entity&& other);

    friend void swap(Entity& a, Entity& b)
    {
        using std::swap;
        swap(a.id_, b.id_);
        swap(a.componentIndices_, b.componentIndices_);
        swap(a.entityManager_, b.entityManager_);
    }

    /*!
     * @brief Add a component to this entity.
     *
     * @param component The component to add to this Entity. The Component
     * type is passed as a template argument. If the Component has a
     * constructor that takes arguments, you may supply those directly as
     * arguments to this function call. E.g.
     * @code
     * myEntity.addComponent<Position2D>(4.2, 6.0);
     * @endcode
     *
     * @note You can only register one instance of every type of component.
     * @return Returns a reference to this Entity. This is to allow chaining.
     */
    template<class T, class... Args>
    Entity& addComponent(Args&&...);

    /*!
     * @brief Remove a component from this entity.
     *
     * The component to be removed is passed as a template argument. E.g.
     * @code
     * myEntity.removeComponent<Position2D>();
     * @endcode
     */
    template<class T>
    Entity& removeComponent();

    Entity& removeAllComponents();

    /*!
     * @brief Get a component from the entity.
     * @return A reference to the requested component.
     */
    template <class T>
    inline T& getComponent() const;

    /*!
     * @brief Checks if the specified component exists.
     * @return True if the entity has the specified component, false if otherwise.
     */
    template <class T>
    bool hasComponent() const;

    /*!
     * @brief Returns true if this entity is supported by the specified system.
     *
     * If the entity owns all the components the specified system has requested
     * then the system is able to support this entity. If the system has an
     * empty list of supported components, then this method will also return
     * true.
     */
    bool supportsSystem(const System&) const;

    /*!
     * @brief Gets this entity's globally unique Identifier.
     */
    ID getID() const;

    /*!
     * @brief Searches the entity's list of components for the templated type
     * and returns the index of that component so it can be retrieved from the
     * entity manager.
     */
    template <class T>
    ONTOLOGY_PRIVATE_API Index mapComponentTypeToIndex() const;

    template <class T>
    ONTOLOGY_PRIVATE_API Index* getComponentIndexPtr();

    template <class T>
    ComponentFactory<T>& getComponentFactory() const;

    ComponentFactoryInterface& getComponentFactory(TypeID componentType) const;

private:
    // Globally unique identifier
    ID id_;

    /*
     * The EntityManager acts as a container for all components, to ensure that
     * components of a particular type are always contiguous in memory. The
     * entity stores the index into that container for every component it has.
     *
     * This is a vector instead of a hashmap because entities don't usually
     * have many components attached to it. Searching small datasets is faster.
     */
    std::vector<std::pair<TypeID, Index>> componentIndices_;

    // A weak ref to the entity manager that owns this entity.
    EntityManager* entityManager_;
};

class EntityPrototype
{
public:
    EntityPrototype(EntityManager* const entityManager);

    template<class T, class... Args>
    EntityPrototype& addComponent(Args...);

    template<class T>
    EntityPrototype& removeComponent();

    template <class T>
    inline T& getComponent() const;

    template <class T>
    bool hasComponent() const;

private:
    friend class EntityManager;

    EntityManager* const                entityManager_;
    std::vector<const std::type_info*>  components_;
};

} // namespace ontology
