//----------------------------------------------------------------------------
//Entity.hpp
//----------------------------------------------------------------------------

#pragma once

//----------------------------------------------------------------------------
// include files

#include <ontology/Component.hpp>
#include <ontology/Entity.hxx>
#include <ontology/EntityManager.hpp>
#include <ontology/Exception.hpp>
#include <ontology/SystemManager.hpp>
#include <ontology/World.hpp>
#include <ontology/Type.hpp>

namespace ontology {

//----------------------------------------------------------------------------
template<class T, class... Args>
Entity& Entity::addComponent(Args&&... args)
{
    ONTOLOGY_ASSERT(hasComponent<T>() == false, DuplicateComponentException, Entity::addComponent<T>,
        std::string("Component of type \"") + TypeID::from<T>().getTypeName() + "\" already registered with this entity"
    )

    Index componentIndex = entityManager_->createComponentForEntity<T>(*this, std::forward<Args>(args)...);
    componentIndices_.emplace_back(TypeID::from<T>(), componentIndex);

    return *this;
}

//----------------------------------------------------------------------------
template<class T>
Entity& Entity::removeComponent()
{
    for (auto it = componentIndices_.begin(); it != componentIndices_.end(); ++it)
        if (it->first == TypeID::from<T>())
        {
            entityManager_->destroyComponentForEntity<T>(*this);
            componentIndices_.erase(it);  // Delete index after destroying. Entity manager makes an index lookup and relies on this existing.
            return *this;
        }

    ONTOLOGY_ASSERT(false, InvalidComponentException, Entity::removeComponent<T>,
        std::string("Component of type \"") + TypeID::from<T>().getTypeName() + "\" is not registered with this entity"
    )

    return *this;
}

//----------------------------------------------------------------------------
template<class T>
inline T& Entity::getComponent() const
{
    return entityManager_->getComponentForEntity<T>(*this);
}

//----------------------------------------------------------------------------
template <class T>
bool Entity::hasComponent() const
{
    return (mapComponentTypeToIndex<T>() != InvalidIndex);
}

//----------------------------------------------------------------------------
template <class T>
Index Entity::mapComponentTypeToIndex() const
{
    for (const auto& pair : componentIndices_)
        if (pair.first == TypeID::from<T>())
            return pair.second;
    return InvalidIndex;
}

//----------------------------------------------------------------------------
template <class T>
Index* Entity::getComponentIndexPtr()
{
    for (auto& pair : componentIndices_)
        if (pair.first == TypeID::from<T>())
            return &pair.second;
    return nullptr;
}

//----------------------------------------------------------------------------
template<class T, class... Args>
EntityPrototype& EntityPrototype::addComponent(Args... args)
{
    entityManager_->registerComponentFactory<T>(args...);
    return *this;
}

//----------------------------------------------------------------------------
template<class T>
EntityPrototype& EntityPrototype::removeComponent()
{
    return *this;
}

//----------------------------------------------------------------------------
template <class T>
inline T& EntityPrototype::getComponent() const
{
    throw std::runtime_error("not implemented");
}

//----------------------------------------------------------------------------
template <class T>
bool EntityPrototype::hasComponent() const
{
    return false;
}

} // namespace ontology
