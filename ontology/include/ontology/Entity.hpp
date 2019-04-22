//-----------------------------------------------------------------------------
//Entity.hpp
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------
// include files

#include <ontology/Component.hpp>
#include <ontology/Entity.hxx>
#include <ontology/EntityManager.hpp>
#include <ontology/Exception.hpp>
#include <ontology/SystemManager.hpp>
#include <ontology/World.hpp>
#include <ontology/Type.hpp>

namespace ontology {

//-----------------------------------------------------------------------------
template<class T, class... Args>
Entity& Entity::addComponent(Args&&... args)
{
    /*
     * We expect the same number of calls to registerComponentFactory as to
     * unregisterComponentFactory. It's possible that different entity prototypes
     * register the same component types, so we have to keep track of how many
     * times the same type is registered so we know when to properly delete it
     * again.
     */

    ONTOLOGY_ASSERT(hasComponent<T>() == false, DuplicateComponentException, Entity::addComponent<T>,
        std::string("Component of type \"") + TypeID::from<T>().getTypeName() + "\" already registered with this entity"
    )

    auto& componentFactories = entityManager_->getComponentFactories();

    // Emplace with nullptr. Create factory if insertion succeeded to avoid unnecessary new/delete calls.
    auto result = componentFactories.emplace(TypeID::from<T>(), nullptr);
    if (result.second)
        result.first->second.reset(new ComponentFactory<T>(entityManager_));
    else
        result.first->second->addRef();  // Factory already exists, increase refcount

    ComponentFactory<T>& factory = *static_cast<ComponentFactory<T>*>(result.first->second.get());

    Index componentIndex = factory.createComponentForEntityWithCustomArgs(*this, std::forward<Args>(args)...);
    componentIndices_.emplace_back(TypeID::from<T>(), componentIndex);

    return *this;
}

//-----------------------------------------------------------------------------
template<class T>
Entity& Entity::removeComponent()
{
    // Search entity vector for type matching component
    TypeID type = TypeID::from<T>();
    auto it = componentIndices_.begin();
    for (; it != componentIndices_.end(); ++it)
        if (it->first == type)
            break;

    ONTOLOGY_ASSERT(it != componentIndices_.end(), InvalidComponentException, Entity::removeComponent<T>,
        std::string("Component of type \"") + TypeID::from<T>().getTypeName() + "\" is not registered with this entity"
    )

    auto& factory = getComponentFactory<T>();
    factory.destroyComponentForEntity(*this);
    componentIndices_.erase(it);

    return *this;
}

//-----------------------------------------------------------------------------
template<class T>
inline T& Entity::getComponent() const
{
    // Search entity vector for type matching component
    TypeID type = TypeID::from<T>();
    auto it = componentIndices_.begin();
    for (; it != componentIndices_.end(); ++it)
        if (it->first == type)
            break;

    ONTOLOGY_ASSERT(it != componentIndices_.end(), InvalidComponentException, Entity::getComponent<T>,
        std::string("Component of type \"") + TypeID::from<T>().getTypeName() + "\" is not registered with this entity"
    )

    auto& factory = getComponentFactory<T>();
    return factory.getComponent(it->second);
}

//-----------------------------------------------------------------------------
template <class T>
bool Entity::hasComponent() const
{
    return (mapComponentTypeToIndex<T>() != InvalidIndex);
}

//-----------------------------------------------------------------------------
template <class T>
Index Entity::mapComponentTypeToIndex() const
{
    for (const auto& pair : componentIndices_)
        if (pair.first == TypeID::from<T>())
            return pair.second;
    return InvalidIndex;
}

//-----------------------------------------------------------------------------
template <class T>
Index* Entity::getComponentIndexPtr()
{
    for (auto& pair : componentIndices_)
        if (pair.first == TypeID::from<T>())
            return &pair.second;
    return nullptr;
}

//-----------------------------------------------------------------------------
template <class T>
ComponentFactory<T>& Entity::getComponentFactory() const
{
    return static_cast<ComponentFactory<T>&>(getComponentFactory(TypeID::from<T>()));
}

// ============================================================================
// ============================================================================

//-----------------------------------------------------------------------------
template<class T, class... Args>
EntityPrototype& EntityPrototype::addComponent(Args... args)
{
    return *this;
}

//-----------------------------------------------------------------------------
template<class T>
EntityPrototype& EntityPrototype::removeComponent()
{
    return *this;
}

//-----------------------------------------------------------------------------
template <class T>
inline T& EntityPrototype::getComponent() const
{
    throw std::runtime_error("not implemented");
}

//-----------------------------------------------------------------------------
template <class T>
bool EntityPrototype::hasComponent() const
{
    return false;
}

} // namespace ontology
