// ----------------------------------------------------------------------------
// System.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/System.hpp>

namespace Ontology {

// ----------------------------------------------------------------------------
System::System() :
    world(nullptr)
{
}

// ----------------------------------------------------------------------------
System::~System()
{
}

// ----------------------------------------------------------------------------
void System::setSupportedComponents(const TypeSet& components)
{
    m_SupportedComponents = components;
}

// ----------------------------------------------------------------------------
const TypeSet& System::getSupportedComponents() const
{
    return m_SupportedComponents;
}

// ----------------------------------------------------------------------------
void System::setDependingSystems(const TypeSet& systems)
{
    m_DependingSystems = systems;
}

// ----------------------------------------------------------------------------
const TypeSet& System::getDependingSystems() const
{
    return m_DependingSystems;
}

// ----------------------------------------------------------------------------
void System::setWorld(World* world)
{
    this->world = world;
}

// ----------------------------------------------------------------------------
void System::informNewEntity(Entity* entity)
{
    for(const auto& it : m_EntityList)
        if(it == entity)
            return;

    if(entity->supportsSystem(*this))
        m_EntityList.push_back(entity);
}

// ----------------------------------------------------------------------------
void System::informDeletedEntity(Entity* entity)
{
    for(auto it = m_EntityList.begin(); it != m_EntityList.end(); ++it)
        if(*it == entity)
            m_EntityList.erase(it);
}

// ----------------------------------------------------------------------------
void System::update(const EntityManager::EntityList& entities) const
{
    for(const auto& it : entities)
        if(it->supportsSystem(*this))
            this->processEntity(*it.get());
}

} // namespace Ontology
