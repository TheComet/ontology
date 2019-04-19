#pragma once

#include "ontology/allocators/Block.hpp"

namespace ontology {

template <intptr_t threshold, class SmallAllocator, class LargeAllocator>
class Segregator : private SmallAllocator, private LargeAllocator
{
public:
    Block allocate(intptr_t size);
    void deallocate(Block block);
    bool owns(Block block) const;
};


// ----------------------------------------------------------------------------
template <intptr_t threshold, class SmallAllocator, class LargeAllocator>
Block Segregator<threshold, SmallAllocator, LargeAllocator>::allocate(intptr_t size)
{
    if(size <= threshold)
        return SmallAllocator::allocate(size);
    return LargeAllocator::allocate(size);
}

// ----------------------------------------------------------------------------
template <intptr_t threshold, class SmallAllocator, class LargeAllocator>
void Segregator<threshold, SmallAllocator, LargeAllocator>::deallocate(Block block)
{
    if(block.size <= threshold)
        return SmallAllocator::deallocate(block);
    LargeAllocator::deallocate(block);
}

// ----------------------------------------------------------------------------
template <intptr_t threshold, class SmallAllocator, class LargeAllocator>
bool Segregator<threshold, SmallAllocator, LargeAllocator>::owns(Block block) const
{
    if(block.size <= threshold)
        return SmallAllocator::owns(block);
    return LargeAllocator::owns(block);
}

} // namespace ontology
