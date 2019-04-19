// ----------------------------------------------------------------------------
// EntityManager.hpp
// ----------------------------------------------------------------------------

#pragma once

#include "ontology/EntityManager.hxx"
#include "ontology/Exception.hpp"
#include "ontology/allocators/Instancer.hpp"
#include "ontology/allocators/Mallocator.hpp"
#include "ontology/Type.hpp"
#include <tuple>
#include <utility>

namespace ontology {

// ----------------------------------------------------------------------------
template <class T, class... DefaultArgs>
void EntityManager::registerComponentFactory(DefaultArgs&&... args)
{
    /* We expect the same number of calls to registerComponentFactory as to
     * unregisterComponentFactory. It's possible that different entity prototypes
     * register the same component types, so we have to keep track of how many
     * times the same type is registered so we know when to properly delete it
     * again.
     */

    // Emplace with nullptr. Create factory if insertion succeeded to avoid unnecessary new/delete calls.
    auto result = componentFactories_.emplace(TypeID::from<T>(), nullptr);

    if (result.second)
        result.first->second.reset(new DefaultComponentFactory<T, DefaultArgs...>(*this, std::forward<DefaultArgs>(args)...));
    else
        result.first->second->addRef();  // Factory already exists, increase refcount
}

// ----------------------------------------------------------------------------
template <class T>
void EntityManager::unregisterComponentFactory()
{
    unregisterComponentFactory(TypeID::from<T>());
}

// ----------------------------------------------------------------------------
template <class T, class... Args>
Index EntityManager::createComponentForEntity(const Entity& entity, Args&&... args)
{
    /*
     * It's possible the component factory does not exist yet. If so, create
     * one without default component arguments.
     */
    auto result = componentFactories_.emplace(TypeID::from<T>(), nullptr);
    if (result.second)
        result.first->second.reset(new ComponentFactory<T>(*this));

    ComponentFactory<T>* factory = static_cast<ComponentFactory<T>*>(result.first->second.get());
    return factory->createComponentForEntityWithCustomArgs(entity, std::forward<Args>(args)...);
}

// ----------------------------------------------------------------------------
template <class T>
void EntityManager::destroyComponentForEntity(const Entity& entity)
{
    ComponentFactory<T>* factory = findComponentFactory<T>();
    factory->destroyComponentForEntity(entity);
}

// ----------------------------------------------------------------------------
template <class T>
T& EntityManager::getComponentForEntity(const Entity& entity)
{
    ComponentFactory<T>* factory = findComponentFactory<T>();
    return factory->getComponentForEntity(entity);
}

// ----------------------------------------------------------------------------
template <class T>
EntityManager::ComponentFactory<T>* EntityManager::findComponentFactory() const
{
    auto result = componentFactories_.find(TypeID::from<T>());
    ONTOLOGY_ASSERT(result != componentFactories_.end(), InvalidComponentException, EntityManager::findComponentFactory<T>, "");

    return static_cast<ComponentFactory<T>*>(result->second.get());
}

// ============================================================================
// ============================================================================

// ----------------------------------------------------------------------------
template <class T>
EntityManager::ComponentFactory<T>::ComponentFactory(EntityManager& entityManager) :
    ComponentFactoryInterface(entityManager)
{
}

// ----------------------------------------------------------------------------
template <class T>
template <class... Args>
Index EntityManager::ComponentFactory<T>::createComponentForEntityWithCustomArgs(const Entity& entity, Args&&... args)
{
    Index insertIndex;
    std::vector<Entity>& el = entityManager_.getEntityList();

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
Index EntityManager::ComponentFactory<T>::createComponentForEntityWithDefaultArgs(const Entity& entity)
{
    assert(false);
    return InvalidIndex;
}

// ----------------------------------------------------------------------------
template <class T>
void EntityManager::ComponentFactory<T>::destroyComponentForEntity(const Entity& e)
{
    Index componentIndex = e.mapComponentTypeToIndex<T>();
    ONTOLOGY_ASSERT(componentIndex != InvalidIndex, InvalidComponentException, destroyComponentForEntity<U>, "");
    components_.erase(components_.begin() + componentIndex);
}

// ----------------------------------------------------------------------------
template <class T>
T& EntityManager::ComponentFactory<T>::getComponent(Index componentIndex)
{
    return components_[componentIndex];
}

// ----------------------------------------------------------------------------
template <class T>
T& EntityManager::ComponentFactory<T>::getComponentForEntity(const Entity& e)
{
    Index componentIndex = e.mapComponentTypeToIndex<T>();
    ONTOLOGY_ASSERT(componentIndex != InvalidIndex, InvalidComponentException, EntityManager::ComponentFactory<T>::getComponentForEntity<T>, "");
    return getComponent(componentIndex);
}

// ============================================================================
// ============================================================================

// ----------------------------------------------------------------------------
template <class T, class... DefaultArgs>
EntityManager::DefaultComponentFactory<T, DefaultArgs...>::DefaultComponentFactory(EntityManager& entityManager, DefaultArgs&&... args) :
    ComponentFactory<T>(entityManager),
    defaultArgs_(std::forward<DefaultArgs>(args)...)
{
}

// ----------------------------------------------------------------------------
template <class T, class... DefaultArgs>
Index EntityManager::DefaultComponentFactory<T, DefaultArgs...>::createComponentForEntityWithDefaultArgs(const Entity& entity)
{
    Index insertIndex = ComponentFactory<T>::components_.size();
    std::apply([this](DefaultArgs... args){
        ComponentFactory<T>::components_.emplace_back(args...);
    }, defaultArgs_);
    return insertIndex;
}

} // namespace ontology
