// ----------------------------------------------------------------------------
// EntityManager.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/Entity.hpp>
#include <ontology/EntityManager.hpp>
#include <ontology/EntityManagerListener.hpp>

#include <string.h>

namespace Ontology {

// ----------------------------------------------------------------------------
EntityManager::EntityManager()
{
    m_EntityListCapacity = m_EntityList.capacity();
}

// ----------------------------------------------------------------------------
EntityManager::~EntityManager()
{
    this->destroyAllEntities();
}

// ----------------------------------------------------------------------------
Entity& EntityManager::createEntity(const char* name)
{
    m_EntityList.emplace_back(name, this);
    this->event.dispatch(&EntityManagerListener::onCreateEntity, m_EntityList.back());
    this->handleEntityReallocation();
    return m_EntityList.back();
}

// ----------------------------------------------------------------------------
void EntityManager::destroyEntity(Entity& entity)
{
    for(auto it = m_EntityList.begin(); it != m_EntityList.end(); ++it)
    {
        if(&(*it) == &entity)
        {
            this->event.dispatch(&EntityManagerListener::onDestroyEntity, entity);
            m_EntityList.erase(it);
            this->handleEntityReallocation(true);
            return;
        }
    }
}

// ----------------------------------------------------------------------------
void EntityManager::destroyEntities(const char* name)
{
    auto it = m_EntityList.begin();
    bool requireReallocationEvent = false;
    while(it != m_EntityList.end())
    {
        if(!strcmp(it->getName(), name))
        {
            this->event.dispatch(&EntityManagerListener::onDestroyEntity, *it);
            it = m_EntityList.erase(it);
            requireReallocationEvent = true;
        }
        else
        {
            ++it;
        }
    }
    
    if(requireReallocationEvent)
        this->handleEntityReallocation(true);
}

// ----------------------------------------------------------------------------
void EntityManager::destroyAllEntities()
{
    auto it = m_EntityList.begin();
    while(it != m_EntityList.end())
    {
        this->event.dispatch(&EntityManagerListener::onDestroyEntity, *it);
        it = m_EntityList.erase(it);
    }
    this->handleEntityReallocation(true);
}

// ----------------------------------------------------------------------------
const EntityManager::EntityList& EntityManager::getEntityList() const
{
    return m_EntityList;
}

// ----------------------------------------------------------------------------
void EntityManager::informAddComponent(Entity& entity, const Component* component) const
{
    this->event.dispatch(&EntityManagerListener::onAddComponent, entity, component);
}

// ----------------------------------------------------------------------------
void EntityManager::informRemoveComponent(Entity& entity, const Component* component) const
{
    this->event.dispatch(&EntityManagerListener::onRemoveComponent, entity, component);
}

// ----------------------------------------------------------------------------
void EntityManager::handleEntityReallocation(bool force)
{
    if(m_EntityListCapacity == m_EntityList.capacity() && !force)
        return;
    this->event.dispatch(&EntityManagerListener::onEntitiesReallocated, m_EntityList);
    m_EntityListCapacity = m_EntityList.capacity();
}

} // namespace Ontology
