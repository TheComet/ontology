// ----------------------------------------------------------------------------
// System.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/World.hpp>
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
void System::informEntityUpdate(Entity& entity)
{
    for(auto& it : m_EntityList)
        if(&it.get() == &entity)
            return;

    if(entity.supportsSystem(*this))
        m_EntityList.push_back(entity);
}

// ----------------------------------------------------------------------------
void System::informDestroyedEntity(const Entity& entity)
{
    for(auto it = m_EntityList.begin(); it != m_EntityList.end(); ++it)
        if(&it->get() == &entity)
            m_EntityList.erase(it);
}

// ----------------------------------------------------------------------------
void System::informEntitiesReallocated(std::vector<Entity>& entityList)
{
    m_EntityList.clear();
    for(auto& it : entityList)
        this->informEntityUpdate(it);
}

// ----------------------------------------------------------------------------
void System::update()
{
    for(auto& it : m_EntityList)
#ifdef ONTOLOGY_MULTITHREADING
        this->world->getIoService().post(
                boost::bind(&System::processEntity, this, boost::ref(it))
        );
        // wait for all entities to be processed
        this->world->getIoService().poll();
#else
        this->processEntity(it);
#endif
}

} // namespace Ontology
