// ----------------------------------------------------------------------------
// System.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_SYSTEM_HPP__
#define __ONTOLOGY_SYSTEM_HPP__

// ----------------------------------------------------------------------------
// include files

#include <ontology/TypeContainers.hpp>
#include <ontology/EntityManager.hpp>

// ----------------------------------------------------------------------------
// forward declarations

namespace Ontology {
    class Entity;
    class World;
}

namespace Ontology {

/*!
 * @brief A system acts upon entities and their components.
 *
 * It is here where all of the logic should be placed. Systems are periodically
 * udpated (when World::update() is called). Override the method
 * System::processEntity() to receive an entity that is ready to be processed
 * as an argument. Override System::initialise() to set up your System class.
 *
 * Each system will only process the entities it can support. Multiple systems
 * can process the same entity. What defines which entities are processed by
 * which systems can be defined when first instantiating the system from the
 * World class.
 *
 * @see World
 * @see SystemManager
 */
class System
{
public:

    /*!
     * @brief Default constructor.
     */
    System();

    /*!
     * @brief Allow destruction through base class pointer.
     */
    virtual ~System();

    /*!
     * @brief Called when an entity requires processing. Override this.
     */
    virtual void processEntity(Entity&) = 0;

    /*!
     * @brief Called when systems should initialise. Override this.
     */
    virtual void initialise() = 0;

    /*!
     * @brief Informs the system of the components it should support.
     */
    void setSupportedComponents(const TypeSet&);

    /*!
     * @brief Gets the typeset of supported components.
     */
    const TypeSet& getSupportedComponents() const;

    /*!
     * @brief Informs the system of the world it is part of.
     */
    void setWorld(World*);

    /*!
     * @brief Called when the system should update all of its entities.
     */
    void update(const EntityManager::EntityList& entityList);

protected:

    /*!
     * @brief Access the world the system belongs to with this.
     */
    World* world;

private:
    TypeSet m_SupportedComponents;
};

} // namespace Ontology

#endif // __ONTOLOGY_SYSTEM_HPP__
