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
Entity& EntityManager::createEntity(const char* name)
{
    m_EntityList.push_back(std::unique_ptr<Entity>(new Entity(name, this)));
    this->event.dispatch(&EntityManagerListener::onCreateEntity, m_EntityList.back().get());
    return *m_EntityList.back().get();
}

// ----------------------------------------------------------------------------
void EntityManager::destroyEntity(const Entity* entity)
{
    for(auto it = m_EntityList.begin(); it != m_EntityList.end(); ++it)
        if(it->get() == entity)
        {
            this->event.dispatch(&EntityManagerListener::onDestroyEntity, entity);
            m_EntityList.erase(it);
            return;
        }
}

// ----------------------------------------------------------------------------
void EntityManager::destroyEntities(const char* name)
{
    auto it = m_EntityList.begin();
    while(it != m_EntityList.end())
    {
        if(!strcmp(it->get()->getName(), name))
            it = m_EntityList.erase(it);
        else
            ++it;
    }
}

// ----------------------------------------------------------------------------
const EntityManager::EntityList& EntityManager::getEntityList() const
{
    return m_EntityList;
}

// ----------------------------------------------------------------------------
void EntityManager::informAddComponent(const Entity* entity, const Component* component) const
{
    this->event.dispatch(&EntityManagerListener::onAddComponent, entity, component);
}

// ----------------------------------------------------------------------------
void EntityManager::informRemoveComponent(const Entity* entity, const Component* component) const
{
    this->event.dispatch(&EntityManagerListener::onRemoveComponent, entity, component);
}

} // namespace Ontology
