#pragma once

#include "ontology/allocators/Block.hpp"
#include "ontology/allocators/AllocatorDefines.hpp"

namespace ontology {

template <intptr_t capacity, intptr_t alignment=DEFAULT_ALIGNMENT>
class StackAllocator
{
public:
    StackAllocator();
    Block allocate(intptr_t size);
    void deallocate(Block block);
    void deallocateAll();
    bool owns(Block block) const;

protected:
    char buffer_[capacity];
    char* ptr_;
};


// ----------------------------------------------------------------------------
template <intptr_t capacity, intptr_t alignment>
StackAllocator<capacity, alignment>::StackAllocator() :
    ptr_(buffer_)
{
}

// ----------------------------------------------------------------------------
template <intptr_t capacity, intptr_t alignment>
Block StackAllocator<capacity, alignment>::allocate(intptr_t size)
{
    Block block;
    if(size <= buffer_ + capacity - ptr_)
    {
        block = Block(size, ptr_);
        ptr_ += size;
    }

    return block;
}

// ----------------------------------------------------------------------------
template <intptr_t capacity, intptr_t alignment>
void StackAllocator<capacity, alignment>::deallocate(Block block)
{
    if(ptr_ == static_cast<char*>(block.buffer) + BLK_SIZE(block))
    {
        ptr_ = static_cast<char*>(block.buffer);
    }
}

// ----------------------------------------------------------------------------
template <intptr_t capacity, intptr_t alignment>
void StackAllocator<capacity, alignment>::deallocateAll()
{
    ptr_ = buffer_;
}

// ----------------------------------------------------------------------------
template <intptr_t capacity, intptr_t alignment>
bool StackAllocator<capacity, alignment>::owns(Block block) const
{
    return block.buffer >= buffer_ && block.buffer < buffer_ + capacity;
}

} // namespace ontology
