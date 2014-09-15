// ----------------------------------------------------------------------------
// SystemManager.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/SystemManager.hpp>

namespace Ontology {

// ----------------------------------------------------------------------------
SystemManager::SystemManager(World* world) :
    m_World(world)
{
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
void SystemManager::update(const EntityManager& entityManager)
{
    for(const auto& it : m_SystemList)
        it.second->update(entityManager.getEntityList());
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
        for(auto ot : it)
            for(auto m : systemLookup)
                if(ot == m.second)
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

    std::vector<const System*> canBeExecutedInParallel;
    canBeExecutedInParallel.push_back(system);
    m_ExecutionList.push_back(std::move(canBeExecutedInParallel));
    return unresolved.erase(unresolved.find(node));
}

// ----------------------------------------------------------------------------
bool SystemManager::isInExecutionList(const System* const system) const
{
    for(const auto& it : m_ExecutionList)
        for(const auto& ot : it)
            if(ot == system)
                return true;
    return false;
}

} // namespace Ontology
