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
bool Entity::supportsSystem(const System& system) const
{/*
    for(const auto& it : system.getSupportedComponents())
        if(m_ComponentMap.find(it) == m_ComponentMap.end())
            return false;*/
    return true;
}

// ----------------------------------------------------------------------------
ID Entity::getID() const
{
    return id_;
}

//----------------------------------------------------------------------------
EntityPrototype::EntityPrototype(EntityManager* const EntityManager) :
    entityManager_(EntityManager)
{
}

} // namespace ontology
