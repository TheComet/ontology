// ----------------------------------------------------------------------------
// SystemManager.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/Config.hpp>
#include <ontology/SystemManager.hpp>
#include <stdexcept>

namespace Ontology {

// ----------------------------------------------------------------------------
/*!
 * @brief Gets the number of cores on this machine.
 * @note see http://www.cprogramming.com/snippets/source-code/find-the-number-of-cpu-cores-for-windows-mac-or-linux
 */
#ifdef ONTOLOGY_MULTITHREADING
#   ifdef ONTOLOGY_PLATFORM_WINDOWS
#       include <windows.h>
#   elif ONTOLOGY_PLATFORM_MAC
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
#   elif ONTOLOGY_PLATFORM_MAC
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
SystemManager::SystemManager(World* world) :
    m_World(world),
    m_CoreCount(1)
{
#ifdef ONTOLOGY_MULTITHREADING
    m_CoreCount = getNumberOfCores();
#   ifdef _DEBUG
    std::cout << "Number of cores: " << m_CoreCount << std::endl;
#   endif
#endif
}

// ----------------------------------------------------------------------------
SystemManager::~SystemManager()
{
    for(auto it = m_SystemList.rbegin(); it != m_SystemList.rend(); ++it)
        it->second.reset(nullptr);
}

// ----------------------------------------------------------------------------
void SystemManager::initialise()
{
    this->computeExecutionOrder();
    for(const auto& it : m_SystemList)
        it.second->initialise();
}

// ----------------------------------------------------------------------------
void SystemManager::update() const
{
    for(const auto& system : m_ExecutionList)
        system->update(m_CoreCount);
}

// ----------------------------------------------------------------------------
void SystemManager::computeExecutionOrder()
{
    // no need to process if no systems are available
    if(!m_SystemList.size())
        return;

    // create temporary lists holding pointers to all systems so lookup time
    // is faster
    TypeMap<const System*> systemLookup;
    TypeSet unresolved;
    for(const auto& it : m_SystemList)
    {
        systemLookup[it.first] = it.second.get();
        unresolved.insert(it.first);
    }

    // dependency resolution
    // see http://www.electricmonk.nl/log/2008/08/07/dependency-resolving-algorithm/
    // make sure starting point has dependencies
    TypeSet resolving;
    TypeSet::iterator systemIt = unresolved.begin();
    while(systemIt != unresolved.end())
    {
        systemIt = this->resolveDependencies(*systemIt, systemLookup, resolving, unresolved);
    }

#ifdef _DEBUG
    std::cout << "final execution order (" << m_ExecutionList.size() << " systems):" << std::endl;
    for(auto it : m_ExecutionList)
        for(auto m : systemLookup)
            if(it == m.second)
            {
                std::cout << "  " << m.first->name() << std::endl;
                break;
            }
#endif
}

// ----------------------------------------------------------------------------
TypeSet::iterator SystemManager::resolveDependencies(const std::type_info* node,
                                        const TypeMap<const System*>& systemLookup,
                                        TypeSet& resolving,
                                        TypeSet& unresolved)
{
    // lookup system referred to by node
    const System* system = systemLookup.find(node)->second;
#ifdef _DEBUG
    std::cout << "Processing dependencies of system " << node->name() << std::endl;
#endif

    // node is unresolved, mark as such
    resolving.insert(node);

    // loop through all depending systems
    for(const auto& edge : system->getDependingSystems())
    {
        // handle depending on a non-registered system type
        const auto& edgeSystemIt = systemLookup.find(edge);
        if(edgeSystemIt == systemLookup.end())
            throw std::runtime_error(std::string("System \"") + node->name() + "\" depends on system \"" + edge->name() + "\", but it isn't registered as a system");
        const System* edgeSystem = edgeSystemIt->second;

#ifdef _DEBUG
        std::cout << "  depends on " << edgeSystemIt->first->name() << std::endl;
#endif

        // don't process edges already part of the final execution list
        if(!this->isInExecutionList(edgeSystem))
        {
            // handle circular dependencies
            if(resolving.find(edge) != resolving.end())
                throw std::runtime_error(std::string("circular dependency detected with systems \"") + node->name() + "\" and \"" + edge->name() + "\"");
            this->resolveDependencies(edge, systemLookup, resolving, unresolved);
        }
#ifdef _DEBUG
        else
            std::cout << "   --> already in execution list" << std::endl;
#endif
    }
    resolving.erase(resolving.find(node));

    m_ExecutionList.push_back(system);
    return unresolved.erase(unresolved.find(node));
}

// ----------------------------------------------------------------------------
bool SystemManager::isInExecutionList(const System* const system) const
{
    for(const auto& it : m_ExecutionList)
        if(it == system)
            return true;
    return false;
}

// ----------------------------------------------------------------------------
void SystemManager::onAddComponent(const Entity* entity, const Component* component)
{
    for(const auto& it : m_SystemList)
        it.second->informEntityUpdate(entity);
}

// ----------------------------------------------------------------------------
void SystemManager::onRemoveComponent(const Entity* entity, const Component* component)
{
    for(const auto& it : m_SystemList)
        it.second->informEntityUpdate(entity);
}

// ----------------------------------------------------------------------------
void SystemManager::onDestroyEntity(const Entity* entity)
{
    for(const auto& it : m_SystemList)
        it.second->informDeletedEntity(entity);
}

} // namespace Ontology
