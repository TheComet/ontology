// ----------------------------------------------------------------------------
// Exception.hpp
// ----------------------------------------------------------------------------

#ifndef __ONTOLOGY_EXCEPTION_HPP__
#define __ONTOLOGY_EXCEPTION_HPP__

// ----------------------------------------------------------------------------
// include files

#include <stdexcept>

namespace Ontology {

class Exception : public std::runtime_error
{
public:
    Exception(const std::string& message);
};

#define DECLARE_EXCEPTION(name) \
struct name : public Exception \
{ \
    name(const std::string& msg) : Exception(msg) {} \
};

DECLARE_EXCEPTION(DuplicateComponentException);
DECLARE_EXCEPTION(InvalidComponentException);
DECLARE_EXCEPTION(CircularDependencyException);

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TO_STRING(__LINE__)
#define ONTOLOGY_ASSERT(class_method, except_type, should_be_true, message) \
        if(! should_be_true) \
                throw except_type(std::string(AT) + " [" + TO_STRING(class_method) + "] Error: " + message);

} // namespace Ontology

#endif //__ONTOLOGY_EXCEPTION_HPP__