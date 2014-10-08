// ----------------------------------------------------------------------------
// System.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/World.hpp>
#include <ontology/Entity.hpp>
#include <ontology/System.hpp>

#ifdef ONTOLOGY_MULTITHREADING
#   include <boost/bind.hpp>
#   include <boost/asio/io_service.hpp>
#   include <boost/thread.hpp>
#endif // ONTOLOGY_MULTITHREADING

namespace Ontology {

// ----------------------------------------------------------------------------
/*!
 * @brief Gets the number of cores on this machine.
 * @note see http://www.cprogramming.com/snippets/source-code/find-the-number-of-cpu-cores-for-windows-mac-or-linux
 */
#ifdef ONTOLOGY_MULTITHREADING
#   if defined(ONTOLOGY_PLATFORM_WINDOWS)
#       include <windows.h>
#   elif defined(ONTOLOGY_PLATFORM_MAC)
#       include <sys/param.h>
#       include <sys/sysctl.h>
#   else
#       include <unistd.h>
#   endif

int getNumberOfCores() {
#   ifdef ONTOLOGY_PLATFORM_WINDOWS
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#   elif defined(ONTOLOGY_PLATFORM_MAC)
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if(count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if(count < 1) { count = 1; }
    }
    return count;
#   else
    return sysconf(_SC_NPROCESSORS_ONLN);
#   endif
}
#endif

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
void System::joinableThreadEntryPoint()
{
    boost::unique_lock<boost::mutex> guard(m_Mutex);
    while(m_ThreadedEntityIterator != m_EntityList.end())
    {
        auto& entity = *m_ThreadedEntityIterator;  // while locked, get reference to current entity
        ++m_ThreadedEntityIterator;                // increment so other threads don't process this entity.
        guard.unlock();
        this->processEntity(entity);
        guard.lock();
    }
    // wakes up main thread, which joins all other threads
    m_ConditionVariable.notify_all();
}

// ----------------------------------------------------------------------------
void System::waitForNotify()
{
    boost::unique_lock<boost::mutex> guard(m_Mutex);
    m_ConditionVariable.wait(guard);
}

// ----------------------------------------------------------------------------
void System::update()
{
    #pragma omp parallel for
    for(auto it = m_EntityList.begin(); it < m_EntityList.end(); ++it)
        this->processEntity(*it);
    return;

    // restart iterator, threads will increment this whenever they pick up
    // a new entity to process until the end is reached.

    m_ThreadedEntityIterator = m_EntityList.begin();
    // create as many threads as there are cores. Threads will automatically
    // begin processing entities.
    boost::thread_group threads;
    for(int i = 0; i != getNumberOfCores(); ++i)
        threads.create_thread(boost::bind(&System::joinableThreadEntryPoint, this));

    // main thread goes idle until all entities are processed. Join all threads
    // when done.
    this->waitForNotify();
    threads.join_all();
}

} // namespace Ontology
