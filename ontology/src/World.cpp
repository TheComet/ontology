// ----------------------------------------------------------------------------
// World.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/World.hpp>
#include <ontology/EntityManager.hpp>
#include <ontology/SystemManager.hpp>
#include <ontology/Entity.hpp>

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

World::World() :
    m_EntityManager(new EntityManager),
    m_SystemManager(new SystemManager(this)),
    m_DeltaTime(0.0)
#ifdef ONTOLOGY_MULTITHREADING
   ,m_IoService(),
    m_ThreadPool(),
    m_Work(m_IoService)
#endif
{
    m_EntityManager->event.addListener(m_SystemManager.get(), "SystemManager");

#ifdef ONTOLOGY_MULTITHREADING
    int cores = getNumberOfCores();
#   ifdef _DEBUG
    std::cout << "Number of cores: " << cores << std::endl;
#   endif
    // populate thread pool with as many threads as there are cores
    for(int i = 0; i != cores; ++i)
        m_ThreadPool.create_thread(
            boost::bind(&boost::asio::io_service::run, &m_IoService)
        );
#endif // ONTOLOGY_MULTITHREADING
}

// ----------------------------------------------------------------------------
World::~World()
{
#ifdef ONTOLOGY_MULTITHREADING
    m_IoService.stop();
    m_ThreadPool.join_all();
#endif

    m_EntityManager->event.removeListener("SystemManager");
}

// ----------------------------------------------------------------------------
EntityManager& World::getEntityManager() const
{
    return *m_EntityManager.get();
}

// ----------------------------------------------------------------------------
SystemManager& World::getSystemManager() const
{
    return *m_SystemManager.get();
}

// ----------------------------------------------------------------------------
void World::setDeltaTime(float deltaTime)
{
    m_DeltaTime = deltaTime;
}

// ----------------------------------------------------------------------------
float World::getDeltaTime() const
{
    return m_DeltaTime;
}

// ----------------------------------------------------------------------------
void World::update()
{
    m_SystemManager->update();
}

// ----------------------------------------------------------------------------
#ifdef ONTOLOGY_MULTITHREADING
boost::asio::io_service& World::getIoService()
{
    return m_IoService;
}
#endif

} // namespace Ontology
