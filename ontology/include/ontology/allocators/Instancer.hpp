#pragma once

#include "ontology/Config.hpp"
#include "ontology/Exception.hpp"
#include <new>
#include <memory>
#include <iostream>

namespace ontology {

template <class Allocator>
class Instancer
{
public:
    template <class T, class... Args>
    T* create(Args&&... args)
    {
        void* memory = allocator_.allocate(sizeof(T));
#if defined(ONTOLOGY_EXCEPTIONS)
        if (memory == nullptr)
            throw std::bad_alloc();
#else
#   if defined(ONTOLOGY_ASSERT)
        assert(memory != nullptr, "Out of memory");
#   endif
        if (memory == nullptr)
            return nullptr;
#endif
        T* instance;
        try {
            instance = new(memory) T(std::forward<Args>(args)...);
        }
        catch (...)
        {
            allocator_.deallocate(memory);
#if defined(ONTOLOGY_EXCEPTIONS)
            throw;
#else
#   if defined(ONTOLOGY_ASSERT)
            assert(false, "Constructor failed");
#   endif
            return nullptr;
#endif
        }

        return instance;
    }

    template <typename T, typename std::enable_if<std::is_polymorphic<T>::value, int>::type=0>
    void destroy(T* instance)
    {
        void* memory = dynamic_cast<void*>(instance);
        instance->~T();
        allocator_.deallocate(memory);
    }

    template <class T, typename std::enable_if<!std::is_polymorphic<T>::value, int>::type=0>
    void destroy(T* instance)
    {
        instance->~T();
        allocator_.deallocate(instance);
    }

    Allocator* getAllocator() { return &allocator_; }

private:
    Allocator allocator_;
};

template <typename Allocator>
class DynamicAlloc
{
public:
    static void* operator new(std::size_t size) { return doAlloc(size); }
    static void* operator new[](std::size_t size) { return doAlloc(size); }
    static void operator delete(void* ptr, std::size_t size) noexcept { return allocator_->deallocate(ptr); }
    static void operator delete[](void* ptr, std::size_t size) noexcept { allocator_->deallocate(ptr); }

    static Allocator* swapAllocator(Allocator* allocator) { std::swap(allocator_, allocator); return allocator; }
    static Allocator* getAllocator() { return allocator_; }

private:
    static void* doAlloc(std::size_t size)
    {
        void* memory = allocator_->allocate(size);
#if defined(ONTOLOGY_EXCEPTIONS)
        if (memory == nullptr)
            throw std::bad_alloc();
#elif defined(ONTOLOGY_ASSERT)
        assert(memory != nullptr, "Out of memory");
#endif
        return memory;
    }

    static Allocator* allocator_;
};

template <typename Allocator>
class StaticAlloc
{
public:
    static void* operator new(std::size_t size) { return doAlloc(size); }
    static void* operator new[](std::size_t size) { return doAlloc(size); }

    static void operator delete(void* ptr, std::size_t size) noexcept { allocator_.deallocate(ptr); }
    static void operator delete[](void* ptr, std::size_t size) noexcept { allocator_.deallocate(ptr); }

    static Allocator* getAllocator() { return &allocator_; }

private:
    static void* doAlloc(std::size_t size)
    {
        void* memory = allocator_.allocate(size);
#if defined(ONTOLOGY_EXCEPTIONS)
        if (memory == nullptr)
            throw std::bad_alloc();
#elif defined(ONTOLOGY_ASSERT)
        assert(memory != nullptr, "Out of memory");
#endif
        return memory;
    }

    static Allocator allocator_;
};

template <typename Allocator>
Allocator* DynamicAlloc<Allocator>::allocator_ = nullptr;

template <typename Allocator>
Allocator StaticAlloc<Allocator>::allocator_;

}
