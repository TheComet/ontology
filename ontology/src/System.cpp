// ----------------------------------------------------------------------------
// System.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/Entity.hpp>
#include <ontology/System.hpp>

#ifdef ONTOLOGY_MULTITHREADING
#include <ctpl/ctpl_stl.h>
#endif

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
void System::informEntityUpdate(const Entity* entity)
{
    for(const auto& it : m_EntityList)
        if(it == entity)
            return;

    if(entity->supportsSystem(*this))
        m_EntityList.push_back(entity);
}

// ----------------------------------------------------------------------------
void System::informDeletedEntity(const Entity* entity)
{
    for(auto it = m_EntityList.begin(); it != m_EntityList.end(); ++it)
        if(*it == entity)
            m_EntityList.erase(it);
}

// ----------------------------------------------------------------------------
void System::update(int coreCount) const
{
#ifdef ONTOLOGY_MULTITHREADING
    ctpl::thread_pool p(coreCount);
    for(const auto& it : m_EntityList)
        p.push([&](int id) { this->processEntity(*it); });
#else
    for(const auto& it : m_EntityList)
        this->processEntity(*it);
#endif
}

} // namespace Ontology
