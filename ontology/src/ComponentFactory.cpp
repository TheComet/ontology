#include "ontology/ComponentFactory.hpp"

namespace ontology {

// ----------------------------------------------------------------------------
ComponentFactoryInterface::ComponentFactoryInterface(EntityManager* entityManager) :
    entityManager_(entityManager)
{
}

// ----------------------------------------------------------------------------
void ComponentFactoryInterface::addRef()
{
    refcount++;
}

// ----------------------------------------------------------------------------
bool ComponentFactoryInterface::subRef()
{
    return (--refcount) == 0;
}

}
