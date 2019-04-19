#include <gmock/gmock.h>
#include "ontology/allocators/FallbackAllocator.hpp"
#include "ontology/allocators/NullAllocator.hpp"

#define NAME ontology_allocators_FallbackAllocator

using namespace ::testing;
using namespace ::ontology;

static intptr_t g_allocWasCalled;
static Block g_deallocWasCalled;
static Block g_ownsWasCalled;

static void reset_globals()
{
    g_allocWasCalled = 0;
    g_deallocWasCalled.ptr = NULL;
    g_deallocWasCalled.size = 0;
    g_ownsWasCalled.ptr = NULL;
    g_ownsWasCalled.size = 0;
}

struct TestAllocator
{
    Block allocate(intptr_t size) { g_allocWasCalled = size; Block b; b.size = size; b.ptr = (void*)1; return b; }
    void deallocate(Block block) { g_deallocWasCalled = block; }
    bool owns(Block block) const { g_ownsWasCalled = block; return true; }
};

struct SuccessAllocator
{
    Block allocate(intptr_t size) { Block b; b.size = 1; b.ptr = (void*)1; return b; }
    void deallocate(Block block) {}
    bool owns(Block block) const { return true; }
};

TEST(NAME, allocate_without_fallback_using_null_allocator)
{
    reset_globals();
    FallbackAllocator<TestAllocator, NullAllocator> fa;
    fa.allocate(20);
    EXPECT_THAT(g_allocWasCalled, Eq(20));
}

TEST(NAME, allocate_without_fallback_using_success_allocator)
{
    reset_globals();
    FallbackAllocator<SuccessAllocator, TestAllocator> fa;
    fa.allocate(20);
    EXPECT_THAT(g_allocWasCalled, Eq(0));
}

TEST(NAME, allocate_with_fallback_using_null_allocator)
{
    reset_globals();
    FallbackAllocator<NullAllocator, TestAllocator> fa;
    fa.allocate(20);
    EXPECT_THAT(g_allocWasCalled, Eq(20));
}

TEST(NAME, deallocate_without_fallback_using_null_allocator)
{
    reset_globals();
    FallbackAllocator<TestAllocator, NullAllocator> fa;
    fa.deallocate(fa.allocate(20));
    EXPECT_THAT(g_deallocWasCalled.size, Eq(20));
}

TEST(NAME, deallocate_without_fallback_using_mallocator)
{
    reset_globals();
    FallbackAllocator<SuccessAllocator, TestAllocator> fa;
    fa.deallocate(fa.allocate(20));
    EXPECT_THAT(g_deallocWasCalled.size, Eq(0));
}

TEST(NAME, deallocate_with_fallback_using_null_allocator)
{
    reset_globals();
    FallbackAllocator<NullAllocator, TestAllocator> fa;
    fa.deallocate(fa.allocate(20));
    EXPECT_THAT(g_deallocWasCalled.size, Eq(20));
}
