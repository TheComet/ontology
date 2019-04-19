#include <gmock/gmock.h>
#include "ontology/allocators/StackAllocator.hpp"
#include "ontology/allocators/NullAllocator.hpp"

#define NAME ontology_allocators_StackAllocator

using namespace ::ontology;
using namespace ::testing;

// Need access to private data
template <intptr_t N, intptr_t M=DEFAULT_ALIGNMENT>
struct TestStackAllocator : public StackAllocator<N, M>
{
    char* getBuffer() { return StackAllocator<N, M>::buffer_; }
    char* getPtr() { return StackAllocator<N, M>::ptr_; }
};

TEST(NAME, allocating_returns_valid_block)
{
    StackAllocator<128> a;
    Block b1 = a.allocate(1);
    Block b2 = a.allocate(5);
    Block b3 = a.allocate(12);

    EXPECT_THAT(b1.ptr, NotNull());
    EXPECT_THAT(b2.ptr, NotNull());
    EXPECT_THAT(b3.ptr, NotNull());
    EXPECT_THAT(b1.size, Eq(1));
    EXPECT_THAT(b2.size, Eq(5));
    EXPECT_THAT(b3.size, Eq(12));
}

TEST(NAME, deallocate_in_reverse_works)
{
    TestStackAllocator<128> a;

    Block b1 = a.allocate(1);
    Block b2 = a.allocate(5);
    Block b3 = a.allocate(12);

    a.deallocate(b3);
    a.deallocate(b2);
    a.deallocate(b1);

    EXPECT_THAT(a.getBuffer(), Eq(a.getPtr()));
}

TEST(NAME, ownership_works)
{
    StackAllocator<128> a;

    Block invalidBlock = NullAllocator().allocate(0);
    Block validBlock = a.allocate(5);

    EXPECT_THAT(a.owns(invalidBlock), Eq(false));
    EXPECT_THAT(a.owns(validBlock), Eq(true));
}

TEST(NAME, deallocate_all_works)
{
    TestStackAllocator<128> a;

    a.allocate(5);
    a.allocate(19);
    a.allocate(34);

    EXPECT_THAT(a.getBuffer(), Ne(a.getPtr()));

    a.deallocateAll();
    EXPECT_THAT(a.getBuffer(), Eq(a.getPtr()));
}

TEST(NAME, allocating_too_much_returns_null_blocks)
{
    StackAllocator<128> a;
    Block success = a.allocate(128);
    Block failure = a.allocate(128);

    EXPECT_THAT(success.ptr, NotNull());
    EXPECT_THAT(success.size, Eq(128));
    EXPECT_THAT(failure.ptr, IsNull());
    EXPECT_THAT(failure.size, Eq(0));
}
