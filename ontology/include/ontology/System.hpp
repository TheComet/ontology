// ----------------------------------------------------------------------------
// System.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_SYSTEM_HPP__
#define __ONTOLOGY_SYSTEM_HPP__

// ----------------------------------------------------------------------------
// include files

#include <ontology/Config.hpp>
#include <ontology/TypeContainers.hpp>

#ifdef ONTOLOGY_MULTITHREADING
#   include <boost/thread/thread.hpp>
#endif

// ----------------------------------------------------------------------------
// forward declarations

namespace Ontology {
    class Entity;
    class World;
}

namespace Ontology {

template <class... T>
inline TypeSet TypeSetGenerator()
{
    return TypeSet({&typeid(T)...});
}

/*!
 * @brief Used to declare that a system should not receive any entities.
 *
 * See SystemManager or SystemManager::addSystem() for more information.
 */
struct None {};

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
class ONTOLOGY_PUBLIC_API System
{
    typedef std::vector< std::reference_wrapper<Entity> > EntityList;
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
    void supportsComponents(const TypeSet&);

    /*!
     * @brief Gets the typeset of supported components.
     */
    ONTOLOGY_LOCAL_API const TypeSet& getSupportedComponents() const;

    /*!
     * @brief Informs the system about which systems need to execute before it.
     */
    void executesAfter(const TypeSet&);

    /*!
     * @brief Gets the typeset of depending systems.
     */
    ONTOLOGY_LOCAL_API const TypeSet& getDependingSystems() const;

    /*!
     * @brief Called by the SystemManager when it receives an update event from an entity.
     *
     * This causes the system to reconsider whether or not it can process the
     * entity in question. If it can, it will add the entity to its internal
     * list of supported entities.
     */
    ONTOLOGY_LOCAL_API void informEntityUpdate(Entity&);

    /*!
     * @brief Called by the SystemManager when it receives an entity destroyed event.
     *
     * This causes the system to remove the entity from its internal list of
     * supported entities, if it exists.
     */
    ONTOLOGY_LOCAL_API void informDestroyedEntity(const Entity&);

    ONTOLOGY_LOCAL_API void informEntitiesReallocated(std::vector<Entity>&);

    /*!
     * @brief Informs the system of the world it is part of.
     */
    ONTOLOGY_LOCAL_API void setWorld(World*);

    /*!
     * @brief Called when the system should update all of its entities.
     */
    ONTOLOGY_LOCAL_API void update();

protected:

    /*!
     * @brief Access the world the system belongs to with this.
     */
    World* world;

private:

    TypeSet         m_SupportedComponents;
    TypeSet         m_DependingSystems;
    EntityList      m_EntityList;

#ifdef ONTOLOGY_MULTITHREADING
    void joinableThreadEntryPoint();
    void waitForNotify();
    boost::condition_variable m_ConditionVariable;
    boost::mutex m_Mutex;
    EntityList::iterator m_ThreadedEntityIterator;
#endif
};

} // namespace Ontology

#endif // __ONTOLOGY_SYSTEM_HPP__
