// ----------------------------------------------------------------------------
// Entity.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/Entity.hpp>
#include <ontology/System.hpp>

namespace ontology {

// ----------------------------------------------------------------------------
Entity::Entity() :
    id_(ID::invalid()),
    entityManager_(nullptr)
{
}

// ----------------------------------------------------------------------------
Entity::Entity(ID id, EntityManager* const entityManager) :
    id_(id),
    entityManager_(entityManager)
{
}

// ----------------------------------------------------------------------------
Entity::~Entity()
{
}

// ----------------------------------------------------------------------------
Entity::Entity(Entity&& other) :
    Entity()
{
    swap(*this, other);
}

// ----------------------------------------------------------------------------
Entity& Entity::operator=(Entity&& other)
{
    swap(*this, other);
    return *this;
}

// ----------------------------------------------------------------------------
Entity& Entity::removeAllComponents()
{
    for (const auto& pair : componentIndices_)
    {
        ComponentFactoryInterface& factory = getComponentFactory(pair.first);
        factory.destroyComponentForEntity(*this);
    }

    componentIndices_.clear();

    return *this;
}

// ----------------------------------------------------------------------------
bool Entity::supportsSystem(const System& system) const
{/*
    for(const auto& it : system.getSupportedComponents())
        if(m_ComponentMap.find(it) == m_ComponentMap.end())
            return false;*/
    return false;
}

// ----------------------------------------------------------------------------
ID Entity::getID() const
{
    return id_;
}

// ----------------------------------------------------------------------------
ComponentFactoryInterface& Entity::getComponentFactory(TypeID componentType) const
{
    EntityManager::ComponentFactoryCollection& factories = entityManager_->getComponentFactories();
    auto result = factories.find(componentType);
    ONTOLOGY_ASSERT(result != factories.end(), InvalidComponentException, Entity::getComponentFactory<T>,
        "Factory for component type \"" + componentType.getTypeName() + "\" not registered with entity manager"
    )
    return *result->second.get();
}

//----------------------------------------------------------------------------
EntityPrototype::EntityPrototype(EntityManager* const EntityManager) :
    entityManager_(EntityManager)
{
}

} // namespace ontology
