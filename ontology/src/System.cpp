// ----------------------------------------------------------------------------
// System.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/Entity.hpp>
#include <ontology/System.hpp>

#ifdef ONTOLOGY_MULTITHREADING
#   include <boost/asio/io_service.hpp>
#   include <boost/bind.hpp>
#endif // ONTOLOGY_MULTITHREADING

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
void System::informEntityUpdate(Entity* entity)
{
    for(const auto& it : m_EntityList)
        if(it == entity)
            return;

    if(entity->supportsSystem(*this))
        m_EntityList.push_back(entity);
}

// ----------------------------------------------------------------------------
void System::informDestroyedEntity(const Entity* entity)
{
    for(auto it = m_EntityList.begin(); it != m_EntityList.end(); ++it)
        if(*it == entity)
            m_EntityList.erase(it);
}

// ----------------------------------------------------------------------------
void System::update(
#ifdef ONTOLOGY_MULTITHREADING
        boost::asio::io_service& ioService
#endif
)
{
    for(auto& it : m_EntityList)
#ifdef ONTOLOGY_MULTITHREADING
        ioService.post(boost::bind(&System::processEntity, this, boost::ref(*it)));
#else
        this->processEntity(*it);
#endif
}

} // namespace Ontology
