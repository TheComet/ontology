// ----------------------------------------------------------------------------
// SystemManager.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_SYSTEM_MANAGER_HPP__
#define __ONTOLOGY_SYSTEM_MANAGER_HPP__

// ----------------------------------------------------------------------------
// include files

#include <ontology/Exception.hpp>
#include <ontology/SystemManager.hxx>
#include <ontology/System.hpp>

namespace Ontology {

// ----------------------------------------------------------------------------
template <class T, class... Args>
System& SystemManager::addSystem(Args&&... args)
{
    ONTOLOGY_ASSERT(m_SystemList.find(&typeid(T)) == m_SystemList.end(), DuplicateSystemException, "SystemManager::addSystem<T, Args...>",
        std::string("System of type \"") + typeid(T).name() + "\" already registered with this manager"
    )

    System* system = new T(args...);
    m_SystemList.emplace_back(
        &typeid(T),
        std::unique_ptr<System>(system)
    );
    this->initSystem(system);
    return *system;
}

// ----------------------------------------------------------------------------
template <class T>
SystemManager& SystemManager::removeSystem()
{
    const auto it = m_SystemList.find(&typeid(T));
    if(it == m_SystemList.end())
        return *this;
    
    m_SystemList.erase(it);
    this->computeExecutionOrder();
    return *this;
}

// ----------------------------------------------------------------------------
template <class T>
T& SystemManager::getSystem()
{
    const auto it = m_SystemList.find(&typeid(T));
    ONTOLOGY_ASSERT(it != m_SystemList.end(), InvalidSystemException, SystemManager::getSystem<T>,
        std::string("System of type \"") + typeid(T).name() + "\" not registered with this manager"
    )
    return *static_cast<T*>(it->second.get());
}

} // namespace Ontology

#endif // __ONTOLOGY_SYSTEM_MANAGER_HPP__
