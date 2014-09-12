// ----------------------------------------------------------------------------
// SystemManager.hpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/System.hpp>
#include <ontology/TypeContainers.hpp>

#include <iostream>
#include <set>
#include <cassert>
#include <initializer_list>

// ----------------------------------------------------------------------------
// forward declarations

namespace Ontology {
    class System;
}

namespace Ontology {

template <class... T>
inline TypeSet SupportsComponents()
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
 * @brief Manages the creation and deletion of systems.
 *
 * The system manager can be retrieved from an instantiated world with
 * World::getSystemManager(). The system is responsible for creating,
 * destroying and updating your systems.
 *
 * Systems can be configured to update only a selection of entities based on
 * the requirements of components. See SystemManager::addSystem for more
 * information.
 * @note SystemManager supports chaining and the programmer is encouraged to
 * use this feature. The following demonstrates the syntax that should be used:
 * @code
 * world.getSystemManager()
 *     .addSystem(new MainLoop,
 *         Ontology::SupportedComponents<
 *             Ontology::None
 *             >())
 *     .addSystem(new MovementSystem,
 *         Ontology::SupportedComponents<
 *             Position,
 *             Velocity
 *             >())
 *     .addSystem(new CollisionSystem
 *         Ontology::SupportedComponents<
 *             Position
 *             >())
 *     .initialise()
 *     ;
 * @endcode
 * Here, MainLoop will not receive any entities at all because Ontology::None
 * was declared. MovementSystem will receive entities that have a Position and
 * Velocity. **CollisionSystem* will receive entities that have a Position.
 *
 * Make sure to call SystemManager::initialise() after adding all of your
 * systems.
 */
class SystemManager
{
public:

    /*!
     * @brief Default constructor.
     */
    SystemManager(World* world);

    /*!
     * @brief Adds a new system to the world.
     *
     * @param system The system to add. SystemManager owns the passed object
     * after calling this and is responsible for deleting it. Make sure to use
     * **new** when instantiating your system.
     * @param supportedComponents Declare which components your system will
     * support. This causes only those entities that actually have all of the
     * components a system requires to be passed to the system's process loop.
     * If you don't specify anything, the system will receive all entities
     * registered to the world.
     *
     * Example:
     * @code
     * world.getSystemManager()
     *    .addSystem(new MovementSystem,
     *        SupportedComponents<
     *            Position,
     *            Velocity
     *            >())
     *    .addSystem(new AnotherSystem)
     *    .initialise()
     *    ;
     * @endcode
     * In this example, MovementSystem will only process entities that have a
     * Position and Velocity component. AnotherSystem will receive all entities
     * registered to the world (since no supported components were declared).
     *
     * @return Returns itself, allowing the programmer to chain.
     */
    template <class T>
    SystemManager& addSystem(T* system, TypeSet&& supportedComponents = std::forward<TypeSet>(TypeSet()))
    {
        assert(m_SystemList.find(&typeid(T)) == m_SystemList.end());
        m_SystemList.push_back(std::pair<const std::type_info*, std::unique_ptr<System> >(
            &typeid(T),
            std::unique_ptr<System>(system))
        );
        system->setSupportedComponents(supportedComponents);
        system->setWorld(m_World);
        return *this;
    }

    /*!
     * @brief Removes the specified system from the world.
     *
     * Example:
     * @code
     * world.getSystemManager().removeSystem<MovementSystem>();
     * @endcode
     */
    template <class T>
    void removeSystem()
    {
        m_SystemList.erase(m_SystemList.find(&typeid(T)));
    }

    /*!
     * @brief Gets the specified system.
     *
     * Example:
     * @code
     * world.getSystemManager().getSystem<MovementSystem>().doThings();
     */
    template <class T>
    T& getSystem()
    {
        return *static_cast<T*>(m_SystemList.find(&typeid(T))->second.get());
    }

    /*!
     * @brief Initialises all currently registered systems.
     */
    void initialise();

    /*!
     * @brief Updates all currently registered systems. Don't call this.
     *
     * @note Don't call this manually, use World::update() instead.
     */
    void update(const EntityManager&);

private:

    TypeVectorPairSmartPtr<System>  m_SystemList;
    TypeSet                         m_supportedComponents;
    World*                          m_World;
};

} // namespace Ontology
