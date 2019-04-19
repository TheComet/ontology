// ----------------------------------------------------------------------------
// Exception.cpp
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// include files

#include <ontology/Exception.hpp>

namespace ontology {

// ----------------------------------------------------------------------------
#if defined(ONTOLOGY_EXCEPTIONS)
Exception::Exception(const std::string& msg) :
    runtime_error(msg)
{
}
#endif

} // namespace ontology
