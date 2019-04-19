#include <gmock/gmock.h>
#include "ontology/allocators/Segregator.hpp"

#define NAME ontology_allocators_Segregator

using namespace ::ontology;
using namespace ::testing;

static intptr_t g_allocWasCalled1;
static Block g_deallocWasCalled1;
static Block g_ownsWasCalled1;
static intptr_t g_allocWasCalled2;
static Block g_deallocWasCalled2;
static Block g_ownsWasCalled2;

static void reset_globals()
{
    g_allocWasCalled1 = 0;
    g_deallocWasCalled1.ptr = NULL;
    g_deallocWasCalled1.size = 0;
    g_ownsWasCalled1.ptr = NULL;
    g_ownsWasCalled1.size = 0;
    g_allocWasCalled2 = 0;
    g_deallocWasCalled2.ptr = NULL;
    g_deallocWasCalled2.size = 0;
    g_ownsWasCalled2.ptr = NULL;
    g_ownsWasCalled2.size = 0;
}

struct TestAllocator1
{
    Block allocate(intptr_t size) { g_allocWasCalled1 = size; Block b; b.size = size; return b; }
    void deallocate(Block block) { g_deallocWasCalled1 = block; }
    bool owns(Block block) const { g_ownsWasCalled1 = block; return true; }
};
struct TestAllocator2
{
    Block allocate(intptr_t size) { g_allocWasCalled2 = size; Block b; b.size = size; return b; }
    void deallocate(Block block) { g_deallocWasCalled2 = block; }
    bool owns(Block block) const { g_ownsWasCalled2 = block; return true; }
};

TEST(NAME, small_allocator_is_called)
{
    reset_globals();

    Segregator<256, TestAllocator1, TestAllocator2> s;
    s.allocate(256);
    EXPECT_THAT(g_allocWasCalled1, Eq(256));
    EXPECT_THAT(g_allocWasCalled2, Eq(0));
}

TEST(NAME, large_allocator_is_called)
{
    reset_globals();

    Segregator<256, TestAllocator1, TestAllocator2> s;
    s.allocate(257);
    EXPECT_THAT(g_allocWasCalled1, Eq(0));
    EXPECT_THAT(g_allocWasCalled2, Eq(257));
}

TEST(NAME, small_deallocator_is_called)
{
    reset_globals();

    Segregator<256, TestAllocator1, TestAllocator2> s;
    s.deallocate(s.allocate(256));
    EXPECT_THAT(g_deallocWasCalled1.size, Eq(256));
    EXPECT_THAT(g_deallocWasCalled2.size, Eq(0));
}

TEST(NAME, large_deallocator_is_called)
{
    reset_globals();

    Segregator<256, TestAllocator1, TestAllocator2> s;
    s.deallocate(s.allocate(257));
    EXPECT_THAT(g_deallocWasCalled1.size, Eq(0));
    EXPECT_THAT(g_deallocWasCalled2.size, Eq(257));
}

TEST(NAME, small_owns_is_called)
{
    reset_globals();

    Segregator<256, TestAllocator1, TestAllocator2> s;
    s.owns(s.allocate(256));
    EXPECT_THAT(g_ownsWasCalled1.size, Eq(256));
    EXPECT_THAT(g_ownsWasCalled2.size, Eq(0));
}

TEST(NAME, large_owns_is_called)
{
    reset_globals();

    Segregator<256, TestAllocator1, TestAllocator2> s;
    s.owns(s.allocate(257));
    EXPECT_THAT(g_ownsWasCalled1.size, Eq(0));
    EXPECT_THAT(g_ownsWasCalled2.size, Eq(257));
}
