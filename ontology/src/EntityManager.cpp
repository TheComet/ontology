// ----------------------------------------------------------------------------
// EntityManager.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/Entity.hpp>
#include <ontology/EntityManager.hpp>
#include <ontology/EntityManagerListener.hpp>
#include "ontology/Type.hpp"
#include "ontology/Exception.hpp"

#include <sstream>
#include <stdexcept>
#include <string.h>

namespace ontology {

// ----------------------------------------------------------------------------
EntityManager::EntityManager(World* world) :
    world_(world)
{
}

// ----------------------------------------------------------------------------
EntityManager::~EntityManager()
{
    destroyAllEntities();
}

// ----------------------------------------------------------------------------
EntityPrototype& EntityManager::addPrototype(std::string name)
{
    auto result = prototypeMap_.emplace(name, EntityPrototype(this));

    ONTOLOGY_ASSERT(result.second, DuplicatePrototypeException, EntityManager::addPrototype,
        "Entity prototype \"" + name + "\" already registered with this entity"
    );

    return result.first->second;
}

// ----------------------------------------------------------------------------
EntityManager& EntityManager::removePrototype(std::string name)
{
    prototypeMap_.erase(name);
    return *this;
}

// ----------------------------------------------------------------------------
Entity& EntityManager::createEntity(std::string prototypeName)
{
    ID newID = world_->generateGUID();
    Index newIndex = entityList_.size();
    std::unordered_map<std::string, EntityPrototype>::const_iterator prototype;

    if (prototypeName.size() > 0)
    {
        prototype = prototypeMap_.find(prototypeName);
        ONTOLOGY_ASSERT(prototype != prototypeMap_.end(), InvalidPrototypeException, EntityManager::createEntity,
            "Entity prototype \"" + prototypeName + "\" isn't registered. Cannot instantiate."
        );
    }

    entityList_.emplace_back(newID, this);
    entityIndexMap_.emplace(newID, newIndex);  // Map entity's ID to the index in the entity list for faster lookup later
    Entity& entity = entityList_.back();

    if (prototypeName.size() > 0)
    {
        for (const auto& component : prototype->second.components_)
        {
            // TODO
        }
    }

    return entity;
}

// ----------------------------------------------------------------------------
EntityManager& EntityManager::destroyEntity(Entity& entity)
{
    return destroyEntity(entity.getID());
}

// ----------------------------------------------------------------------------
EntityManager& EntityManager::destroyEntity(ID entityID)
{
    auto entityIndexIt = entityIndexMap_.find(entityID);
    ONTOLOGY_ASSERT(entityIndexIt != entityIndexMap_.end(), InvalidEntityException, EntityManager::destroyEntity,
        "Entity with ID " + entityID.to_string() + " not found"
    );

    Index entityIndex = entityIndexIt->second;
    auto entityIt = entityList_.begin() + entityIndex;

    entityIt->removeAllComponents();

    entityList_.erase(entityIt);
    entityIndexMap_.erase(entityIndexIt);

    // Index map (maps entity IDs to indices in the vector) needs to be updated,
    // as all entities above the one deleted have shifted down by one.
    for (auto& entry : entityIndexMap_)
        if (entry.second > entityIndex)
            entry.second--;

    return *this;
}

// ----------------------------------------------------------------------------
EntityManager& EntityManager::destroyAllEntities()
{
    // TODO Destroy components

    entityIndexMap_.clear();
    entityList_.clear();

    return *this;
}

// ----------------------------------------------------------------------------
Entity& EntityManager::getEntity(ID entityID)
{
    return entityList_[entityIDToIndex(entityID)];
}

// ----------------------------------------------------------------------------
Entity& EntityManager::getEntity(Index entityIdx)
{
    return entityList_[entityIdx];
}

// ----------------------------------------------------------------------------
Index EntityManager::entityIDToIndex(ID entityID)
{
    auto it = entityIndexMap_.find(entityID);
    ONTOLOGY_ASSERT(it != entityIndexMap_.end(), InvalidEntityException, EntityManager::destroyEntity,
        "Entity with ID " + entityID.to_string() + " is not registered with this manager"
    );

    return it->second;
}

// ----------------------------------------------------------------------------
std::vector<Entity>& EntityManager::getEntityList()
{
    return entityList_;
}

// ----------------------------------------------------------------------------
EntityManager::ComponentFactoryCollection& EntityManager::getComponentFactories()
{
    return componentFactories_;
}

} // namespace ontology
