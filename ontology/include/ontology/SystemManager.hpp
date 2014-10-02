// ----------------------------------------------------------------------------
// SystemManager.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_SYSTEM_MANAGER_HPP__
#define __ONTOLOGY_SYSTEM_MANAGER_HPP__

// ----------------------------------------------------------------------------
// include files

#include <ontology/SystemManager.hxx>
#include <ontology/System.hpp>

namespace Ontology {

// ----------------------------------------------------------------------------
template <class T, class... Args>
SystemManager& SystemManager::addSystem(Args&&... args,
                         TypeSet supportedComponents,
                         TypeSet executesBeforeSystems)
{
    // add to system list
    assert(m_SystemList.find(&typeid(T)) == m_SystemList.end());
    System* system = new T(args...);
    m_SystemList.emplace_back(
        &typeid(T),
        std::unique_ptr<System>(system)
    );
    system->setWorld(m_World);
    system->setSupportedComponents(supportedComponents);
    // default behaviour is to depend on previously registered system
    /*if(!executesBeforeSystems.size() && m_SystemList.size() > 1)
        executesBeforeSystems = TypeSet({(m_SystemList.end()-2)->first});*/
    system->setDependingSystems(executesBeforeSystems);
    return *this;
}

// ----------------------------------------------------------------------------
template <class T>
void SystemManager::removeSystem()
{
    m_SystemList.erase(m_SystemList.find(&typeid(T)));
    this->computeExecutionOrder();
}

// ----------------------------------------------------------------------------
template <class T>
T& SystemManager::getSystem()
{
    return *static_cast<T*>(m_SystemList.find(&typeid(T))->second.get());
}

} // namespace Ontology

#endif // __ONTOLOGY_SYSTEM_MANAGER_HPP__
