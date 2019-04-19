#include <gmock/gmock.h>
#include "ontology/allocators/FallbackAllocator.hpp"
#include "ontology/allocators/FreeList.hpp"
#include "ontology/allocators/Mallocator.hpp"
#include "ontology/allocators/NullAllocator.hpp"
#include "ontology/allocators/Segregator.hpp"
#include "ontology/allocators/StackAllocator.hpp"

using namespace ::testing;
using namespace ::ontology;

