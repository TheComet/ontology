//----------------------------------------------------------------------------
//Entity.hpp
//----------------------------------------------------------------------------

#ifndef __ONTOLOGY_ENTITY_HPP__
#define __ONTOLOGY_ENTITY_HPP__

//----------------------------------------------------------------------------
//includefiles

#include<ontology/Component.hpp>
#include<ontology/TypeContainers.hpp>

#include<map>
#include<typeinfo>
#include<string>
#include<cassert>
#include<memory>

//----------------------------------------------------------------------------
//forwarddeclarations

namespace Ontology{
    class System;
}

namespace Ontology{

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
 *     .addComponent(new Position(0, 0))
 *     .addComponent(new Velocity(1, 2))
 *     .addComponent(new Sprite("player.png"))
 *     ;
 * @endcode
 * @see Component
 */
class Entity
{
public:

    /*!
     * @brief Construct an entity with a name.
     */
    Entity(const char* name);

    /*!
     * @brief Allow destruction through base class pointer.
     */
    virtual ~Entity();

    /*!
     * @brief Add a component to this entity.
     *
     * @param component The component to add. Entity owns the passed object
     * after calling this and is responsible for deleting it. Make sure to use
     * **new** when instantiating your component.
     *
     * @note You can only register one instance of every type of component.
     * @return Returns a reference to itself. This is to allow chaining.
     */
    template<class T>
    Entity& addComponent(T* component)
    {
        assert(m_ComponentMap.find(&typeid(T)) == m_ComponentMap.end());
        m_ComponentMap[&typeid(T)] = std::unique_ptr<Component>(component);
        return*this;
    }

    /*!
     * @brief Remove a component from this entity.
     */
    template<class T>
    void removeComponent()
    {
        m_ComponentMap.erase(m_ComponentMap.find(&typeid(T)));
    }

    /*!
     * @brief Get a component from the entity.
     * @return A pointer to the requested component.
     */
    template<class T>
    T& getComponent()
    {
        auto it = m_ComponentMap.find(&typeid(T));
        assert(it != m_ComponentMap.end());
        return *static_cast<T*>(it->second.get());
    }

    /*!
     * @brief Returns true if this entity is supported by the specified system.
     *
     * If the entity owns all the components the specified system has requested
     * then the system is able to support this entity. If the system has an
     * empty list of supported components, then this method will also return
     * true.
     */
    bool supportsSystem(const System&);

    /*!
     * @brief Gets the name of the entity.
     * @return A const char pointer to the entity's name string.
     */
    const char* getName() const;

private:

    TypeMapSmartPtr<Component>  m_ComponentMap;
    const char*                 m_Name;
};

} // namespace Ontology

#endif //__ONTOLOGY_ENTITY_HPP__
