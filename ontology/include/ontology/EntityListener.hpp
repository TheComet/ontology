// ----------------------------------------------------------------------------
// EntityListener.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_ENTITY_LISTENER_HPP__
#define __ONTOLOGY_ENTITY_LISTENER_HPP__

// ----------------------------------------------------------------------------
// include files

namespace ontology {

class EntityListener
{
public:

    /*!
     * @brief Allow destruction through base class pointer.
     */
    virtual ~EntityListener()
    {}

    virtual void onAddComponent(const Component*)
    {}

    virtual void onRemoveComponent(const Component*)
    {}
};

} // namespace ontology

#endif // __ONTOLOGY_ENTITY_LISTENER_HPP__
