// ----------------------------------------------------------------------------
// EntityManager.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/EntityManager.hpp>

#include <string.h>

namespace Ontology {

// ----------------------------------------------------------------------------
Entity& EntityManager::createEntity(const char* name)
{
    m_EntityList.push_back(std::unique_ptr<Entity>(new Entity(name)));
    return *m_EntityList.back().get();
}

// ----------------------------------------------------------------------------
void EntityManager::destroyEntity(const Entity* entity)
{
    for(auto it = m_EntityList.begin(); it != m_EntityList.end(); ++it)
        if(it->get() == entity)
        {
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

} // namespace Ontology
