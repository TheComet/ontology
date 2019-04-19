// ----------------------------------------------------------------------------
// EntityManagerListener.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_ENTITY_MANAGER_LISTENER_HPP__
#define __ONTOLOGY_ENTITY_MANAGER_LISTENER_HPP__

// ----------------------------------------------------------------------------
// include files

#include <ontology/Config.hpp>

#include <vector>

// ----------------------------------------------------------------------------
// forward declarations

namespace ontology {
    class Entity;
}

namespace ontology {

/*!
 * @brief Listener interface for classes that want to listen to EntityManager events.
 */
class ONTOLOGY_PUBLIC_API EntityManagerListener
{
public:

    /*!
     * @brief Called when a new entity was created.
     * @param entity The new entity.
     */
    virtual void onCreateEntity(Entity& entity);

    /*!
     * @brief Called when an entity is destroyed.
     * @param entity The entity being destroyed.
     */
    virtual void onDestroyEntity(Entity& entity);
};

} // namespace ontology

#endif // __ONTOLOGY_ENTITY_MANAGER_LISTENER_HPP__
