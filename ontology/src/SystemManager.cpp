// ----------------------------------------------------------------------------
// SystemManager.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/SystemManager.hpp>

namespace Ontology {

// ----------------------------------------------------------------------------
SystemManager::SystemManager(World* world) :
    m_World(world)
{
}

// ----------------------------------------------------------------------------
SystemManager::~SystemManager()
{
    for(auto it = m_SystemList.rbegin(); it != m_SystemList.rend(); ++it)
        it->second.reset(nullptr);
}

// ----------------------------------------------------------------------------
void SystemManager::initialise()
{
    for(const auto& it : m_SystemList)
        it.second->initialise();
}

// ----------------------------------------------------------------------------
void SystemManager::update(const EntityManager& entityManager)
{
    for(const auto& it : m_SystemList)
        it.second->update(entityManager.getEntityList());
}

} // namespace Ontology
