#include <gmock/gmock.h>
#include "ontology/allocators/Mallocator.hpp"

#define NAME ontology_allocators_Mallocator

using namespace ::ontology;
using namespace ::testing;

TEST(NAME, allocating_returns_valid_block)
{
    Mallocator a;
    Block b1 = a.allocate(1);
    Block b2 = a.allocate(5);
    Block b3 = a.allocate(10000);

    EXPECT_THAT(b1.ptr, NotNull());
    EXPECT_THAT(b2.ptr, NotNull());
    EXPECT_THAT(b3.ptr, NotNull());
    EXPECT_THAT(b1.size, Eq(1));
    EXPECT_THAT(b2.size, Eq(5));
    EXPECT_THAT(b3.size, Eq(10000));

    a.deallocate(b3);
    a.deallocate(b2);
    a.deallocate(b1);
}
