#pragma once

#include "ontology/allocators/Block.hpp"
#include <limits>

namespace ontology {

template <class A,
    size_t maxListLength=-1,
    size_t minSize=std::numeric_limits<size_t>::min(),
    size_t maxSize=std::numeric_limits<size_t>::max()
>
class FreeList
{
public:
    FreeList();

    Block allocate(size_t size);
    void deallocate(Block block);
    bool owns(Block block) const;

private:
    struct Node
    {
        Node* next;
    }* root_;
    size_t listLength_;
    A parentAllocator_;
};


// ----------------------------------------------------------------------------
template <class A, size_t maxListLength, size_t minSize, size_t maxSize>
FreeList<A, maxListLength, minSize, maxSize>::FreeList() :
    root_(NULL),
    listLength_(0)
{
}

// ----------------------------------------------------------------------------
template <class A, size_t maxListLength, size_t minSize, size_t maxSize>
Block FreeList<A, maxListLength, minSize, maxSize>::allocate(size_t size)
{
    if(size >= minSize && size <= maxSize && listLength_ > 0)
    {
        Block block = {root_, size};
        root_ = root_->next;
        --listLength_;
        return block;
    }

    return parentAllocator_.allocate(size);
}

// ----------------------------------------------------------------------------
template <class A, size_t maxListLength, size_t minSize, size_t maxSize>
void FreeList<A, maxListLength, minSize, maxSize>::deallocate(Block block)
{
    if(block.size < minSize || block.size > maxSize || listLength_ == maxListLength)
        return parentAllocator_.deallocate(block);

    Node* p = (Node*)block.ptr;
    p->next = root_;
    root_ = p;
    ++listLength_;
}

// ----------------------------------------------------------------------------
template <class A, size_t maxListLength, size_t minSize, size_t maxSize>
bool FreeList<A, maxListLength, minSize, maxSize>::owns(Block block) const
{
    return (block.size >= minSize && block.size <= maxSize) || parentAllocator_.owns(block);
}

} // namespace ontology
