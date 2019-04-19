#include "ontology/allocators/Mallocator.hpp"
#include <cstdlib>

namespace ontology {

// ----------------------------------------------------------------------------
void* Mallocator::allocate(std::size_t size)
{
    return malloc(size);
}

// ----------------------------------------------------------------------------
void Mallocator::deallocate(void* block)
{
    free(block);
}

}
