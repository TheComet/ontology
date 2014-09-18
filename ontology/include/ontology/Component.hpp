// ----------------------------------------------------------------------------
// Component.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_COMPONENT_HPP__
#define __ONTOLOGY_COMPONENT_HPP__

// ----------------------------------------------------------------------------
// include files

#include <ontology/Export.hpp>

namespace Ontology {

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

class ONTOLOGY_API Component
{
public:

    /*!
     * @brief Allow destruction through base class pointer.
     */
    virtual ~Component();

protected:

    /*!
     * @brief Disallow construction without inheritance.
     */
    Component();
};

} // namespace Ontology

#endif // __ONTOLOGY_COMPONENT_HPP__
