// ----------------------------------------------------------------------------
// Type.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include "ontology/Type.hpp"

namespace ontology {

#if defined(__GNUG__)
#   include <cstdlib>
#   include <memory>
#   include <cxxabi.h>

std::string TypeID::getTypeName() const {

    int status = -4; // some arbitrary value to eliminate the compiler warning

    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(ti->name(), NULL, NULL, &status),
        std::free
    };

    return (status==0) ? res.get() : ti->name();
}

#else

// does nothing if not g++
std::string TypeID::getTypeName() const {
    return ti->name();
}

#endif

}
