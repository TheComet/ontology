#pragma once

#include "ontology/allocators/Block.hpp"

namespace ontology {

template <class Primary, class Fallback>
class FallbackAllocator : private Primary, private Fallback
{
public:
    Block allocate(intptr_t size);
    void deallocate(Block block);
    bool owns(Block block) const;
};


template <class Primary, class Fallback>
Block FallbackAllocator<Primary, Fallback>::allocate(intptr_t size)
{
    Block block = Primary::allocate(size);
    if(block.ptr == NULL)
        block = Fallback::allocate(size);
    return block;
}

template <class Primary, class Fallback>
void FallbackAllocator<Primary, Fallback>::deallocate(Block block)
{
    if(Primary::owns(block))
        Primary::deallocate(block);
    else
        Fallback::deallocate(block);
}

template <class Primary, class Fallback>
bool FallbackAllocator<Primary, Fallback>::owns(Block block) const
{
    return Primary::owns(block) || Fallback::owns(block);
}

} // namespace ontology
