// ----------------------------------------------------------------------------
// SystemManagerListener.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_SYSTEM_MANAGER_LISTENER_HPP__
#define __ONTOLOGY_SYSTEM_MANAGER_LISTENER_HPP__

// ----------------------------------------------------------------------------
// forward declarations

namespace Ontology {
    class System;
}

namespace Ontology {

class SystemManagerListener
{
public:
    virtual void newSystem(System* system)
    {}
};

} // namespace Ontology

#endif // __ONTOLOGY_SYSTEM_MANAGER_LISTENER_HPP__
