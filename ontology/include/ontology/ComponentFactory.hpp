#pragma once

#include "ontology/ComponentFactory.hxx"
#include "ontology/EntityManager.hxx"
#include "ontology/Entity.hxx"
#include "ontology/Exception.hpp"

namespace ontology {

// ----------------------------------------------------------------------------
template <class T>
ComponentFactory<T>::ComponentFactory(EntityManager* entityManager) :
    ComponentFactoryInterface(entityManager)
{
}

// ----------------------------------------------------------------------------
template <class T>
template <class... Args>
Index ComponentFactory<T>::createComponentForEntityWithCustomArgs(const Entity& entity, Args&&... args)
{
    Index insertIndex;
    std::vector<Entity>& el = entityManager_->getEntityList();

    // Assume we are inserting last
    insertIndex = components_.size();

    // We are not the most recently created entity
    if (&entity != &el.back())
    {
        // Probe forwards in list of entities and find the next entity that has
        // the component type we want to create. This will be the index in which
        // to insert the component, if found. If not found, then we insert last.
        for (Index entityIdx = &entity - el.data() + 1; entityIdx != el.size(); ++entityIdx)
        {
            const Entity& nextEntity = el[entityIdx];
            Index componentIndex = nextEntity.mapComponentTypeToIndex<T>();
            if (componentIndex != InvalidIndex)
            {
                insertIndex = componentIndex;
                break;
            }
        }
    }

    components_.emplace(components_.begin() + insertIndex, std::forward<Args>(args)...);

    // Indices of all entities that have components above the insertion index
    // have changed due to insertion. Need to fix those.
    if (insertIndex != components_.size() - 1)
    {
        for (auto& e : el)
        {
            Index* componentIndex = e.getComponentIndexPtr<T>();
            if (componentIndex == nullptr)
                continue;
            if (*componentIndex >= insertIndex)
                (*componentIndex)++;
        }
    }

    return insertIndex;
}

// ----------------------------------------------------------------------------
template <class T>
Index ComponentFactory<T>::createComponentForEntityWithDefaultArgs(const Entity& entity)
{
    assert(false);
    return InvalidIndex;
}

// ----------------------------------------------------------------------------
template <class T>
void ComponentFactory<T>::destroyComponentForEntity(const Entity& entity)
{
    Index eraseIndex = entity.mapComponentTypeToIndex<T>();
    std::vector<Entity>& el = entityManager_->getEntityList();
    ONTOLOGY_ASSERT(eraseIndex != InvalidIndex, InvalidComponentException, destroyComponentForEntity<U>, "");

    components_.erase(components_.begin() + eraseIndex);

    // We are not the most recently created entity
    if (&entity != &el.back())
    {
        for (auto& e : el)
        {
            Index* componentIndex = e.getComponentIndexPtr<T>();
            if (componentIndex == nullptr)
                continue;
            if (*componentIndex >= eraseIndex)
                (*componentIndex)--;
        }
    }
}

// ----------------------------------------------------------------------------
template <class T>
T& ComponentFactory<T>::getComponent(Index componentIndex)
{
    return components_[componentIndex];
}

// ----------------------------------------------------------------------------
template <class T>
T& ComponentFactory<T>::getComponentForEntity(const Entity& e)
{
    Index componentIndex = e.mapComponentTypeToIndex<T>();
    ONTOLOGY_ASSERT(componentIndex != InvalidIndex, InvalidComponentException, EntityManager::ComponentFactory<T>::getComponentForEntity<T>, "");
    return getComponent(componentIndex);
}

// ----------------------------------------------------------------------------
template <class T, class... DefaultArgs>
DefaultComponentFactory<T, DefaultArgs...>::DefaultComponentFactory(EntityManager* entityManager, DefaultArgs&&... args) :
    ComponentFactory<T>(entityManager),
    defaultArgs_(std::forward<DefaultArgs>(args)...)
{
}

// ----------------------------------------------------------------------------
template <class T, class... DefaultArgs>
Index DefaultComponentFactory<T, DefaultArgs...>::createComponentForEntityWithDefaultArgs(const Entity& entity)
{
    Index insertIndex = ComponentFactory<T>::components_.size();
    std::apply([this](DefaultArgs... args){
        ComponentFactory<T>::components_.emplace_back(args...);
    }, defaultArgs_);
    return insertIndex;
}

}
