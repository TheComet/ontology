// ----------------------------------------------------------------------------
// System.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_SYSTEM_HPP__
#define __ONTOLOGY_SYSTEM_HPP__

// ----------------------------------------------------------------------------
// include files

#include <ontology/System.hxx>

namespace Ontology {

// ----------------------------------------------------------------------------
template <class... T>
inline TypeSet TypeSetGenerator()
{
    return TypeSet({&typeid(T)...});
}

// ----------------------------------------------------------------------------
template <class... T>
inline void System::supportsComponents()
{
    m_SupportedComponents = TypeSetGenerator<T...>();
}

// ----------------------------------------------------------------------------
template <class... T>
inline void System::executesAfter()
{
    m_DependingSystems = TypeSetGenerator<T...>();
}

} // namespace Ontology

#endif // __ONTOLOGY_SYSTEM_HPP__