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
T& SystemManager::addSystem(Args&&... args)
{
    assert(m_SystemList.find(&typeid(T)) == m_SystemList.end());

    T* system = new T(args...);
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
    m_SystemList.erase(m_SystemList.find(&typeid(T)));
    this->computeExecutionOrder();
    return *this;
}

// ----------------------------------------------------------------------------
template <class T>
T& SystemManager::getSystem()
{
    return *static_cast<T*>(m_SystemList.find(&typeid(T))->second.get());
}

} // namespace Ontology

#endif // __ONTOLOGY_SYSTEM_MANAGER_HPP__
