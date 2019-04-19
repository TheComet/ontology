// ----------------------------------------------------------------------------
// Type.hpp
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// include files

#include <set>
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeinfo>

namespace ontology {

typedef uint32_t Index;

static const Index InvalidIndex = static_cast<Index>(-1);

struct ID
{
    explicit ID(uint32_t value) : value(value) {}
    static ID invalid() { return ID(-1); }
    operator uint32_t() const { return value; }

    std::string to_string() const { return std::to_string(value); }

    uint32_t value;
};

class TypeID
{
public:
    /*!
     * @brief Entity comparator for entry in std::set and std::map.
     * @note see http://stackoverflow.com/questions/8682582/what-is-type-infobefore-useful-for
     */
    struct Comparator
    {
        inline bool operator()(const TypeID& a, const TypeID& b) const
        {
            return a.ti->before(*b.ti);
        }
    };

    template <class T>
    static TypeID from() { return TypeID(&typeid(T)); }

    std::string getTypeName() const;

private:
    friend struct std::hash<TypeID>;
    friend bool operator==(const TypeID& lhs, const TypeID& rhs);

    TypeID(const std::type_info* ti) : ti(ti) {}
    const std::type_info* ti;
};

inline bool operator==(const TypeID& lhs, const TypeID& rhs) { return lhs.ti == rhs.ti; }

/// A set of std::type_info pointers (TypeID::from<T>()).
typedef std::set<TypeID, TypeID::Comparator> TypeInfoSet;

/// A map of std::type_info pointers (TypeID::from<T>()) and a templated type.
template <class T>
struct GenericTypeInfoMap
{
    typedef std::unordered_map<TypeID, T> TypeInfoMap;
};
template <class T>
using TypeInfoMap = typename GenericTypeInfoMap<T>::TypeInfoMap;

/// A TypeMap the second pair type being a unique_ptr of a templated type.
template <class T>
using TypeInfoMapUniquePtr = typename GenericTypeInfoMap< std::unique_ptr<T> >::TypeInfoMap;

/// A TypeMap the second pair type being a shared_ptr of a templated type.
template <class T>
using TypeInfoMapSharedPtr = typename GenericTypeInfoMap< std::shared_ptr<T> >::TypeInfoMap;

/// A vector of std::type_info pointers (TypeID::from<T>())
typedef std::vector<const TypeID&> TypeInfoVector;

/// A pair of std::type_info pointers (TypeID::from<T>()) and a templated type.
template <class T>
using TypeInfoPair = std::pair<const TypeID&, T>;

/// A vector of templated TypePairs.
template <class T>
using TypeInfoPairVector = std::vector< TypeInfoPair<T> >;

/// A TypeVectorPair wrapping a templated smart pointer type
template <class SmartPtr>
struct TypeVectorPairSmartPtr : public TypeInfoPairVector<SmartPtr>
{
    typedef typename TypeInfoPairVector<SmartPtr>::iterator iterator;
    typedef typename TypeInfoPairVector<SmartPtr>::const_iterator const_iterator;

    /// Searches the vector for the specified type. (linear search)
    iterator find(const TypeID& key)
    {
        iterator it = this->begin();
        for(; it != this->end(); ++it)
            if(it->first == key)
                return it;
        return it;
    }

    /// Searches the vector for the specified type. (linear search)
    const_iterator find(const TypeID& key) const
    {
        const_iterator it = this->begin();
        for(; it != this->end(); ++it)
            if(it->first == key)
                return it;
        return it;
    }
};

/// A TypeVectorPair with the second pair type being a unique_ptr of a templated type.
template <class T>
using TypeVectorPairUniquePtr = TypeVectorPairSmartPtr< std::unique_ptr<T> >;

/// A TypeVectorPair with the second pair type being a shared_ptr of a templated type.
template <class T>
using TypeVectorPairSharedPtr = TypeVectorPairSmartPtr< std::shared_ptr<T> >;

}

namespace std
{
    template<>
    struct hash<ontology::ID>
    {
        typedef ontology::ID argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& o) const
        {
            return std::hash<uint32_t>{}(o.value);
        }
    };

    template<>
    struct hash<ontology::TypeID>
    {
        typedef ontology::TypeID argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& o) const
        {
            return std::hash<const type_info*>{}(o.ti);
        }
    };
}
