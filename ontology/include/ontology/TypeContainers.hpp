// ----------------------------------------------------------------------------
// TypeContainers.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_TYPE_COMPARATOR_HPP__
#define __ONTOLOGY_TYPE_COMPARATOR_HPP__

// ----------------------------------------------------------------------------
// include files

#include <typeinfo>
#include <vector>
#include <set>
#include <map>
#include <memory>

namespace Ontology {

/*!
 * @brief Entity comparator for entry in std::set and std::map.
 * @note see http://stackoverflow.com/questions/8682582/what-is-type-infobefore-useful-for
 */
struct TypeComparator
{
    inline bool operator()(const std::type_info* a, const std::type_info* b) const
    {
        return a->before(*b);
    }
};

/// A set of std::type_info pointers (&typeid(T)).
typedef typename std::set<const std::type_info*, TypeComparator> TypeSet;

/// A map of std::type_info pointers (&typeid(T)) and a templated type.
template <class T>
struct GenericTypeMap
{
    typedef typename std::map<const std::type_info*, T, TypeComparator> TypeMap;
};
template <class T>
using TypeMap = typename GenericTypeMap<T>::TypeMap;

/// A TypeMap the second pair type being a unique_ptr of a templated type.
template <class T>
using TypeMapSmartPtr = typename GenericTypeMap< std::unique_ptr<T> >::TypeMap;

/// A vector of std::type_info pointers (&typeid(T))
typedef typename std::vector<const std::type_info*> TypeVector;

/// A pair of std::type_info pointers (&typeid(T)) and a templated type.
template <class T>
using TypePair = typename std::pair<const std::type_info*, T>;

/// A vector of templated TypePairs.
template <class T>
using TypeVectorPair = typename std::vector< TypePair<T> >;

/// A TypeVectorPair with the second pair type being a unique_ptr of a templated type.
template <class T>
class TypeVectorPairSmartPtr : public TypeVectorPair< std::unique_ptr<T> >
{
public:
    typedef typename TypeVectorPair< std::unique_ptr<T> >::iterator iterator;

    /// Searches the vector for the specified type. (linear search)
    iterator find(const std::type_info* key)
    {
        iterator it = this->begin();
        for(; it != this->end(); ++it)
            if(it->first == key)
                return it;
        return it;
    }
};

} // namespace Ontology

#endif // __ONTOLOGY_TYPE_COMPARATOR_HPP__
