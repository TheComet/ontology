#include <gmock/gmock.h>
#include "ontology/allocators/NullAllocator.hpp"
#include "ontology/allocators/Mallocator.hpp"

#define NAME ontology_allocators_NullAllocator

using namespace ::ontology;
using namespace ::testing;

TEST(NAME, allocating_returns_null)
{
    NullAllocator a;
    Block b1 = a.allocate(0);
    Block b2 = a.allocate(5);
    Block b3 = a.allocate(10000);

    EXPECT_THAT(b1.ptr, IsNull());
    EXPECT_THAT(b2.ptr, IsNull());
    EXPECT_THAT(b3.ptr, IsNull());
    EXPECT_THAT(b1.size, Eq(0));
    EXPECT_THAT(b2.size, Eq(0));
    EXPECT_THAT(b3.size, Eq(0));
}

TEST(NAME, deallocating_null_blocks_works)
{
    NullAllocator a;
    a.deallocate(a.allocate(5));
}

#if defined(DEBUG)
TEST(NAME, deallocating_non_null_blocks_fails_in_debug_mode)
{
    NullAllocator a1;
    Mallocator a2;

    Block b = a2.allocate(5);
    ASSERT_THAT(b.ptr, NotNull());
    EXPECT_DEATH(a1.deallocate(b), ".*?");

    a2.deallocate(b);
}
#endif

TEST(NAME, ownership_works)
{
    NullAllocator a;
    Block b = a.allocate(5);
    EXPECT_THAT(a.owns(b), Eq(true));
}
