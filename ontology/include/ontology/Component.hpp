// ----------------------------------------------------------------------------
// Component.hpp
// ----------------------------------------------------------------------------

#ifndef ONTOLOGY_COMPONENT_HPP
#define ONTOLOGY_COMPONENT_HPP

// ----------------------------------------------------------------------------
// include files

#include <ontology/Config.hpp>
#include <ontology/allocators/Instancer.hpp>
#include <ontology/allocators/Mallocator.hpp>

namespace ontology {

/*!
 * @brief A component represents data.
 *
 * Components should have little to no actual functionality other than to
 * store data. An example would be a Vector2 class, or maybe a Sprite class.
 *
 * Example:
 * @code
 * struct Position : public Ontology::Component
 * {
 *     Position(float x, float y) : x(x), y(y) {}
 *     float x;
 *     float y;
 *     // overload some operators for arithmetic
 * };
 * @endcode
 */
class Component
{
public:
    virtual ~Component() {}
};

} // namespace ontology

#endif // ONTOLOGY_COMPONENT_HPP
