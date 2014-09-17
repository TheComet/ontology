// ----------------------------------------------------------------------------
// World.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/World.hpp>
#include <ontology/EntityManager.hpp>
#include <ontology/SystemManager.hpp>

namespace Ontology {

// ----------------------------------------------------------------------------
World::World() :
    m_EntityManager(new EntityManager),
    m_SystemManager(new SystemManager(this)),
    m_DeltaTime(0.0)
{
    m_EntityManager->event.addListener(m_SystemManager.get(), "SystemManager");
}

// ----------------------------------------------------------------------------
World::~World()
{
}

// ----------------------------------------------------------------------------
EntityManager& World::getEntityManager() const
{
    return *m_EntityManager.get();
}

// ----------------------------------------------------------------------------
SystemManager& World::getSystemManager() const
{
    return *m_SystemManager.get();
}

// ----------------------------------------------------------------------------
void World::setDeltaTime(float deltaTime)
{
    m_DeltaTime = deltaTime;
}

// ----------------------------------------------------------------------------
float World::getDeltaTime() const
{
    return m_DeltaTime;
}

// ----------------------------------------------------------------------------
void World::update()
{
    m_SystemManager->update(this->getEntityManager());
}

} // namespace Ontology
