// ----------------------------------------------------------------------------
// World.hpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/Export.hpp>

#include <vector>
#include <memory>

#ifdef ONTOLOGY_MULTITHREADING
#   include <boost/asio/io_service.hpp>
#   include <boost/bind.hpp>
#   include <boost/thread/thread.hpp>
#endif // ONTOLOGY_MULTITHREADING

// ----------------------------------------------------------------------------
// forward declarations

namespace Ontology {
    class EntityManager;
    class SystemManager;
}

namespace Ontology {

/*!
 * @brief The world is the entry point to the framework.
 * Usually one instance of World is enough, but there's no reason not to have
 * multiple worlds. Instances of Entity, Component and System cannot be
 * transferred between worlds. What can be done is to dispatch listener events
 * from one system in one world to another system in another world. They will
 * have to register to themselves in order for this to work.
 *
 * World has two main parts. It has an EntityManager and a SystemManager. These
 * are responsible for adding and removing entities, components and systems to
 * your world.
 *
 * The world also stores a delta time, which can be accessed from within any
 * registered system.
 */
class ONTOLOGY_API World
{
public:

    /*!
     * @brief Default constructor.
     */
    World();

    /*!
     * @brief Default destructor.
     */
    ~World();

    /*!
     * @brief Gets this world's entity manager.
     */
    EntityManager& getEntityManager() const;

    /*!
     * @brief Gets this world's system manager.
     */
    SystemManager& getSystemManager() const;

    /*!
     * @brief Sets the world's delta time.
     *
     * The delta time is usually used by systems via World::getDeltaTime().
     * when processing entities.
     * @param delta Delta time to set.
     */
    void setDeltaTime(float delta);

    /*!
     * @brief Gets the world's delta time.
     */
    float getDeltaTime() const;

    /*!
     * @brief Update all systems.
     */
    void update();

#ifdef ONTOLOGY_MULTITHREADING
    boost::asio::io_service& getIoService();
#endif

private:
    std::unique_ptr<EntityManager>  m_EntityManager;
    std::unique_ptr<SystemManager>  m_SystemManager;
    float                           m_DeltaTime;

    // threading
#ifdef ONTOLOGY_MULTITHREADING
    boost::asio::io_service         m_IoService;
    boost::thread_group             m_ThreadPool;
    boost::asio::io_service::work   m_Work;
#endif // ONTOLOGY_MULTITHREADING
};

} // namespace Ontology
