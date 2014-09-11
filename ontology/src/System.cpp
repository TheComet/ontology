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
void System::setWorld(World* world)
{
    this->world = world;
}

// ----------------------------------------------------------------------------
void System::update(const EntityManager::EntityList& entities)
{
    for(const auto& it : entities)
        if(it->supportsSystem(*this))
            this->processEntity(*it.get());
}

} // namespace Ontology
