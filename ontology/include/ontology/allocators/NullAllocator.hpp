#pragma once

#include "ontology/Config.hpp"
#include <cstddef>

namespace ontology {

class NullAllocator
{
public:
    void* allocate(std::size_t size);
    void deallocate(void* block);
    bool owns(void* block) const;
};

}
