#pragma once

#include <cstdint>

namespace ontology {

class Mallocator
{
public:
    void* allocate(std::size_t size);
    void deallocate(void* block);
};

} // namespace ontology
