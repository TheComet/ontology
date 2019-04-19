#include <gmock/gmock.h>
#include "ontology/allocators/Instancer.hpp"
#include "ontology/allocators/NullAllocator.hpp"

#define NAME ontology_allocators_Instancer

using namespace ::ontology;
using namespace ::testing;

struct CallbackInterface
{
    virtual ~CallbackInterface() {}
    virtual void onConstruct(int id=0) = 0;
    virtual void onDestruct(int id=0) = 0;
};

struct CustomAllocatorInterface
{
    virtual ~CustomAllocatorInterface() {}
    virtual void* allocate(std::size_t size) = 0;
    virtual void deallocate(void* block) = 0;
};

struct MockCallback : public CallbackInterface
{
    MOCK_METHOD1(onConstruct, void(int));
    MOCK_METHOD1(onDestruct, void(int));
};

struct MockCustomAllocator : public CustomAllocatorInterface
{
    MOCK_METHOD1(allocate, void*(std::size_t));
    MOCK_METHOD1(deallocate, void(void*));
};

struct CustomAllocator;
struct Obj : public StaticAlloc<CustomAllocator>
{
    Obj() : cb_(nullptr) {}
    Obj(CallbackInterface* cb) : cb_(cb) { cb_->onConstruct(); }
    ~Obj() { if (cb_) cb_->onDestruct(); }

    CallbackInterface* cb_;
};

struct Callback : public CallbackInterface
{
    void onConstruct(int) override {}
    void onDestruct(int) override {}
};

struct CustomAllocator : public CustomAllocatorInterface
{
    CustomAllocator() : freeOffset_(0) {}

    void* allocate(size_t size) override
    {
        void* block = memory_ + freeOffset_;
        freeOffset_ += sizeof(Obj);
        return block;
    }

    void deallocate(void* block) override
    {
        freeOffset_ -= sizeof(Obj);
    }

    char memory_[sizeof(Obj) * 3];
    size_t freeOffset_;
};

#if defined(ONTOLOGY_EXCEPTIONS)
TEST(NAME, operator_new_throws_bad_alloc_if_allocator_returns_null)
{
    struct Obj : public DynamicAlloc<CustomAllocatorInterface> {};

    MockCustomAllocator mockAllocator;
    EXPECT_CALL(mockAllocator, allocate(sizeof(Obj)))
        .WillOnce(Return(nullptr));

    Obj::swapAllocator(&mockAllocator);
    EXPECT_THROW(new Obj, std::bad_alloc);
    Obj::swapAllocator(nullptr);
}

TEST(NAME, operator_new_array_throws_bad_alloc_if_allocator_returns_null)
{
    struct Obj : public DynamicAlloc<CustomAllocatorInterface> {};

    MockCustomAllocator mockAllocator;

    EXPECT_CALL(mockAllocator, allocate(_))
        .WillOnce(Return(nullptr));

    Obj::swapAllocator(&mockAllocator);
    EXPECT_THROW(new Obj[3], std::bad_alloc);
    Obj::swapAllocator(nullptr);
}

TEST(NAME, instancer_throws_bad_alloc_if_allocator_returns_null)
{
    struct Obj {};

    Instancer<MockCustomAllocator> inst;
    EXPECT_CALL(*inst.getAllocator(), allocate(sizeof(Obj)))
        .WillOnce(Return(nullptr));

    EXPECT_THROW(inst.create<Obj>(), std::bad_alloc);
}

TEST(NAME, operator_new_deallocate_is_called_if_constructor_throws)
{
    char memory[32];
    struct Obj : public DynamicAlloc<CustomAllocatorInterface> {
        Obj(CallbackInterface* cb) : cb_(cb) { cb_->onConstruct(); throw std::runtime_error("oh no"); }
        ~Obj() { cb_->onDestruct(); }
        CallbackInterface* cb_;
    };

    MockCustomAllocator mockAllocator;
    MockCallback cb;
    EXPECT_CALL(mockAllocator, allocate(sizeof(Obj))).WillOnce(Return(memory));  // We expect memory to successfully allocate
    EXPECT_CALL(mockAllocator, deallocate(static_cast<void*>(memory))).Times(1);  // Since constructor throws, expect memory to be deallocated again
    EXPECT_CALL(cb, onConstruct(_)).Times(1);  // Constructor should be called (so we can throw)
    EXPECT_CALL(cb, onDestruct(_)).Times(0);  // Destructor should NOT be called, because exception is thrown

    Obj::swapAllocator(&mockAllocator);
    EXPECT_THROW(new Obj(&cb), std::runtime_error);
    Obj::swapAllocator(nullptr);
}

TEST(NAME, operator_new_array_deallocate_is_called_if_constructor_throws)
{
    static int idCounter = 1;
    struct Obj : public DynamicAlloc<CustomAllocatorInterface> {
        Obj(CallbackInterface* cb) : cb_(cb), id_(idCounter++) {
            cb_->onConstruct(id_);
            if (id_ == 3)
                throw std::runtime_error("oh no");
        }
        ~Obj() { cb_->onDestruct(id_); }
        CallbackInterface* cb_;
        int id_;
    };

    char memory[sizeof(Obj) * 6];

    MockCustomAllocator mockAllocator;
    MockCallback cb;
    EXPECT_CALL(mockAllocator, allocate(_)).WillOnce(Return(memory));  // We expect memory to successfully allocate
    EXPECT_CALL(mockAllocator, deallocate(static_cast<void*>(memory))).Times(1);  // Since constructor throws, expect memory to be deallocated again
    EXPECT_CALL(cb, onConstruct(1)).Times(1);
    EXPECT_CALL(cb, onConstruct(2)).Times(1);
    EXPECT_CALL(cb, onConstruct(3)).Times(1);
    EXPECT_CALL(cb, onDestruct(1)).Times(1);
    EXPECT_CALL(cb, onDestruct(2)).Times(1);
    EXPECT_CALL(cb, onDestruct(3)).Times(0);

    Obj::swapAllocator(&mockAllocator);
    EXPECT_THROW((new Obj[3]{&cb, &cb, &cb}), std::runtime_error);
    Obj::swapAllocator(nullptr);
}

TEST(NAME, instancer_deallocate_is_called_if_constructor_throws)
{
    char memory[32];
    struct Obj {
        Obj(CallbackInterface* cb) : cb_(cb) { cb_->onConstruct(); throw std::runtime_error("oh no"); }
        ~Obj() { cb_->onDestruct(); }
        CallbackInterface* cb_;
    };

    Instancer<MockCustomAllocator> inst;
    MockCallback cb;
    EXPECT_CALL(*inst.getAllocator(), allocate(sizeof(Obj))).WillOnce(Return(memory));  // We expect memory to successfully allocate
    EXPECT_CALL(*inst.getAllocator(), deallocate(static_cast<void*>(memory))).Times(1);  // Since constructor throws, expect memory to be deallocated again
    EXPECT_CALL(cb, onConstruct(_)).Times(1);  // Constructor should be called (so we can throw)
    EXPECT_CALL(cb, onDestruct(_)).Times(0);  // Destructor should NOT be called, because exception is thrown

    EXPECT_THROW(inst.create<Obj>(&cb), std::runtime_error);
}
#endif

TEST(NAME, operator_new_allocate_and_deallocate_are_called)
{
    // Want to test whether an object inheriting from one of the *Alloc
    // classes calls that allocator's methods when instantiating with new/delete

    struct Obj : public DynamicAlloc<CustomAllocatorInterface> {};
    char memory[32];

    MockCustomAllocator mockAllocator;

    EXPECT_CALL(mockAllocator, allocate(sizeof(Obj)))
        .WillOnce(Return(memory));
    EXPECT_CALL(mockAllocator, deallocate(memory))
        .Times(1);

    Obj::swapAllocator(&mockAllocator);

    Obj* obj = new Obj;
    delete obj;

    Obj::swapAllocator(nullptr);
}

TEST(NAME, operator_new_constructor_and_destructor_are_called)
{
    // Want to test whether the constructor/destruct is called when instantiating
    // an object using new/delete inheriting from one of the *Alloc classes

    MockCallback cb;
    EXPECT_CALL(cb, onConstruct(_)).Times(1);
    EXPECT_CALL(cb, onDestruct(_)).Times(1);

    Obj* obj = new Obj(&cb);
    void* expectedMemoryAddress = Obj::getAllocator()->memory_;
    ASSERT_THAT(obj, Eq(expectedMemoryAddress));
    delete obj;
}

TEST(NAME, operator_new_array_allocate_and_deallocate_are_called)
{
    // Want to test whether an object inheriting from one of the *Alloc
    // classes calls that allocator's methods when instantiating with new/delete

    struct Obj : public DynamicAlloc<CustomAllocatorInterface> {};
    char memory[32];

    MockCustomAllocator mockAllocator;

    EXPECT_CALL(mockAllocator, allocate(_))
        .WillOnce(Return(memory));
    EXPECT_CALL(mockAllocator, deallocate(_))
        .Times(1);

    Obj::swapAllocator(&mockAllocator);

    Obj* obj = new Obj[3];
    delete[] obj;

    Obj::swapAllocator(nullptr);
}

TEST(NAME, operator_new_array_constructors_and_destructors_are_called)
{
    // Want to test whether the constructor/destruct is called when instantiating
    // an object using new/delete inheriting from one of the *Alloc classes

    MockCallback cb;
    EXPECT_CALL(cb, onConstruct(_)).Times(Exactly(3));
    EXPECT_CALL(cb, onDestruct(_)).Times(Exactly(3));

    Obj* obj = new Obj[3]{&cb, &cb, &cb};
    delete[] obj;
}

TEST(NAME, instancer_allocate_and_deallocate_are_called)
{
    struct Obj {};
    char memory[32];

    Instancer<MockCustomAllocator> inst;
    EXPECT_CALL(*inst.getAllocator(), allocate(sizeof(Obj)))
        .WillOnce(Return(memory));
    EXPECT_CALL(*inst.getAllocator(), deallocate(static_cast<void*>(memory)))
        .Times(Exactly(1));

    Obj* obj = inst.create<Obj>();
    ASSERT_THAT(obj, Eq(static_cast<void*>(memory)));
    inst.destroy(obj);
}

TEST(NAME, instancer_constructor_and_destructor_are_called)
{
    struct Obj {
        Obj(CallbackInterface* cb) : cb_(cb) { cb_->onConstruct(); }
        ~Obj() { cb_->onDestruct(); }
        CallbackInterface* cb_;
    };

    MockCallback cb;
    EXPECT_CALL(cb, onConstruct(_)).Times(1);
    EXPECT_CALL(cb, onDestruct(_)).Times(1);

    Instancer<CustomAllocator> inst;
    Obj* obj = inst.create<Obj>(&cb);
    inst.destroy(obj);
}

TEST(NAME, operator_new_deallocate_through_second_base_pointer)
{
    char memory[32];
    struct Base1 { virtual ~Base1() {} int x; };
    struct Base2 { virtual ~Base2() {} int y; };
    struct Derived : Base1, Base2, DynamicAlloc<CustomAllocatorInterface> { int z; };

    MockCustomAllocator mockAllocator;
    EXPECT_CALL(mockAllocator, allocate(sizeof(Derived))).WillOnce(Return(memory));
    EXPECT_CALL(mockAllocator, deallocate(static_cast<void*>(memory))).Times(1);

    Derived::swapAllocator(&mockAllocator);
    Derived* d = new Derived;
    Base2* b2 = d;  // Memory address of b2 will be different than that of d
    EXPECT_THAT(reinterpret_cast<void*>(b2), Ne(reinterpret_cast<void*>(d)));
    delete b2;
    Derived::swapAllocator(nullptr);
}

TEST(NAME, instancer_deallocate_through_second_base_pointer)
{
    char memory[32];
    struct Base1 { virtual ~Base1() {} int x; };
    struct Base2 { virtual ~Base2() {} int y; };
    struct Derived : Base1, Base2 { int z; };

    Instancer<MockCustomAllocator> inst;
    EXPECT_CALL(*inst.getAllocator(), allocate(sizeof(Derived))).WillOnce(Return(memory));
    EXPECT_CALL(*inst.getAllocator(), deallocate(static_cast<void*>(memory))).Times(1);

    Derived* d = inst.create<Derived>();
    Base2* b2 = d;  // Memory address of b2 will be different than that of d
    EXPECT_THAT(reinterpret_cast<void*>(b2), Ne(reinterpret_cast<void*>(d)));
    inst.destroy(b2);
}
