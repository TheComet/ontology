#include <gmock/gmock.h>
#include "ontology/allocators/AlignedStackAllocator.hpp"
#include "ontology/allocators/NullAllocator.hpp"

#define NAME allocators_AlignedStackAllocator

using namespace ::ontology;
using namespace ::testing;

// Need access to private data
template <intptr_t N, intptr_t M=DEFAULT_ALIGNMENT>
struct TestAlignedStackAllocator : public AlignedStackAllocator<N, M>
{
    char* getBuffer() { return AlignedStackAllocator<N, M>::buffer_; }
    char* getPtr() { return AlignedStackAllocator<N, M>::ptr_; }
};

TEST(NAME, allocating_returns_valid_block)
{
    AlignedStackAllocator<128> a;
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
    TestAlignedStackAllocator<128> a;

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
    AlignedStackAllocator<128> a;

    Block invalidBlock = NullAllocator().allocate(0);
    Block validBlock = a.allocate(5);

    EXPECT_THAT(a.owns(invalidBlock), Eq(false));
    EXPECT_THAT(a.owns(validBlock), Eq(true));
}

TEST(NAME, deallocate_all_works)
{
    TestAlignedStackAllocator<128> a;

    a.allocate(5);
    a.allocate(19);
    a.allocate(34);

    EXPECT_THAT(a.getBuffer(), Ne(a.getPtr()));

    a.deallocateAll();
    EXPECT_THAT(a.getBuffer(), Eq(a.getPtr()));
}

TEST(NAME, allocating_too_much_returns_null_blocks)
{
    AlignedStackAllocator<128> a;
    Block success = a.allocate(128);
    Block failure = a.allocate(128);

    EXPECT_THAT(success.ptr, NotNull());
    EXPECT_THAT(success.size, Eq(128));
    EXPECT_THAT(failure.ptr, IsNull());
    EXPECT_THAT(failure.size, Eq(0));
}

TEST(NAME, alignment_is_correct)
{
    AlignedStackAllocator<128, 4> a1;
    AlignedStackAllocator<128, 16> a2;

    Block b11 = a1.allocate(1);
    Block b12 = a1.allocate(2);
    Block b13 = a1.allocate(7);
    Block b14 = a1.allocate(5);

    Block b21 = a2.allocate(1);
    Block b22 = a2.allocate(6);
    Block b23 = a2.allocate(31);
    Block b24 = a2.allocate(23);

    EXPECT_THAT((char*)b12.ptr - (char*)b11.ptr, Eq(4));
    EXPECT_THAT((char*)b14.ptr - (char*)b13.ptr, Eq(8));
    EXPECT_THAT((char*)b22.ptr - (char*)b21.ptr, Eq(16));
    EXPECT_THAT((char*)b24.ptr - (char*)b23.ptr, Eq(32));
}
