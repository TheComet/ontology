// ----------------------------------------------------------------------------
// System.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_SYSTEM_HPP__
#define __ONTOLOGY_SYSTEM_HPP__

// ----------------------------------------------------------------------------
// include files

#include <ontology/System.hxx>

namespace ontology {

// ----------------------------------------------------------------------------
template <class... T>
inline TypeInfoSet TypeSetGenerator()
{
    return TypeInfoSet({TypeID::from<T>()...});
}

// ----------------------------------------------------------------------------
template <class... T>
inline System& System::supportsComponents()
{
    m_SupportedComponents = TypeSetGenerator<T...>();
    return *this;
}

// ----------------------------------------------------------------------------
template <class... T>
inline System& System::executesAfter()
{
    m_DependingSystems = TypeSetGenerator<T...>();
    return *this;
}

} // namespace ontology

#endif // __ONTOLOGY_SYSTEM_HPP__
