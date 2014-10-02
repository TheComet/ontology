// ----------------------------------------------------------------------------
// Entity.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/Entity.hpp>
#include <ontology/System.hpp>

namespace Ontology {

// ----------------------------------------------------------------------------
Entity::Entity(const char* name, const EntityManagerInterface* creator) :
    m_Name(name),
    m_Creator(creator)
{
}

// ----------------------------------------------------------------------------
Entity::~Entity()
{
}

// ----------------------------------------------------------------------------
bool Entity::supportsSystem(const System& system) const
{/*
    for(const auto& it : system.getSupportedComponents())
        if(m_ComponentMap.find(it) == m_ComponentMap.end())
            return false;*/
    return true;
}

// ----------------------------------------------------------------------------
const char* Entity::getName() const
{
    return m_Name;
}

} // namespace Ontology
