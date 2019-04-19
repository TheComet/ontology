#include "ontology/allocators/NullAllocator.hpp"

namespace ontology {

// ----------------------------------------------------------------------------
void* NullAllocator::allocate(std::size_t size)
{
    return nullptr;
}

// ----------------------------------------------------------------------------
void NullAllocator::deallocate(void* block)
{
#if defined(ONTOLOGY_ASSERT)
    assert(block == nullptr);
#endif
}

// ----------------------------------------------------------------------------
bool NullAllocator::owns(void* block) const
{
    return (block == nullptr);
}

}
