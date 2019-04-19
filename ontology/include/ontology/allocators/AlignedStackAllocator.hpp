#pragma once

#include "ontology/allocators/Block.hpp"
#include "ontology/allocators/AllocatorDefines.hpp"

namespace ontology {

template <intptr_t capacity, intptr_t alignment=DEFAULT_ALIGNMENT>
class AlignedStackAllocator
{
public:
    AlignedStackAllocator();
    Block allocate(intptr_t size);
    void deallocate(Block block);
    void deallocateAll();
    bool owns(Block block) const;
    intptr_t roundToAligned(intptr_t size) const;

protected:
    char buffer_[capacity];
    char* ptr_;
};


// ----------------------------------------------------------------------------
template <intptr_t capacity, intptr_t alignment>
AlignedStackAllocator<capacity, alignment>::AlignedStackAllocator() :
    ptr_(buffer_)
{
}

// ----------------------------------------------------------------------------
template <intptr_t capacity, intptr_t alignment>
Block AlignedStackAllocator<capacity, alignment>::allocate(intptr_t size)
{
    Block block;
    intptr_t aligned = roundToAligned(size);
    if(aligned > buffer_ + capacity - ptr_)
    {
        block.ptr = NULL;
        block.size = 0;
    }
    else
    {
        block.ptr = ptr_;
        block.size = size;
        ptr_ += aligned;
    }

    return block;
}

// ----------------------------------------------------------------------------
template <intptr_t capacity, intptr_t alignment>
void AlignedStackAllocator<capacity, alignment>::deallocate(Block block)
{
    if(ptr_ == (char*)block.ptr + roundToAligned(block.size))
    {
        ptr_ = (char*)block.ptr;
    }
}

// ----------------------------------------------------------------------------
template <intptr_t capacity, intptr_t alignment>
void AlignedStackAllocator<capacity, alignment>::deallocateAll()
{
    ptr_ = buffer_;
}

// ----------------------------------------------------------------------------
template <intptr_t capacity, intptr_t alignment>
bool AlignedStackAllocator<capacity, alignment>::owns(Block block) const
{
    return block.ptr >= buffer_ && block.ptr < buffer_ + capacity;
}

// ----------------------------------------------------------------------------
template <intptr_t capacity, intptr_t alignment>
intptr_t AlignedStackAllocator<capacity, alignment>::roundToAligned(intptr_t size) const
{
    //return (size + alignment - 1) / alignment * alignment;
    // WARNING Only works for alignments that are power-of-two
    return (size + alignment - 1) & -alignment;
}

} // namespace ontology
