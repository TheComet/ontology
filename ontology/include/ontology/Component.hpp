// ----------------------------------------------------------------------------
// Component.hpp
// ----------------------------------------------------------------------------

#ifndef ONTOLOGY_COMPONENT_HPP
#define ONTOLOGY_COMPONENT_HPP

// ----------------------------------------------------------------------------
// include files

#include <ontology/Config.hpp>

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

class ONTOLOGY_PUBLIC_API Component
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

#endif // ONTOLOGY_COMPONENT_HPP
