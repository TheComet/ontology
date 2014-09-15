// --------------------------------------------------------------
// define export macros
// --------------------------------------------------------------

#ifndef __ONTOLOGY_EXPORT_HPP__
#   define __ONTOLOGY_EXPORT_HPP__

    // headers
#   include <ontology/Config.hpp>

#   if defined(ONTOLOGY_DYNAMIC)
#       define ONTOLOGY_API ONTOLOGY_EXPORT
#   else
#       define ONTOLOGY_API ONTOLOGY_IMPORT
#   endif

#endif // __ONTOLOGY_EXPORT_HPP__
