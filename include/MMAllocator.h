#pragma once
#include <cstddef>
#include <limits>
#include <new>

#include "MemoryManager.h"

template<typename T>
class MMAllocator
{
public:
    using value_type = T;

    MMAllocator() noexcept = default;

    template<typename U>
    MMAllocator(const MMAllocator<U>&) noexcept
    {
    }

    T* allocate(std::size_t n)
    {
        if (n == 0)
        {
            return nullptr;
        }

        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }

        std::size_t size = n * sizeof(T);

        void* ptr = MemoryManager::Instance().Allocate(size);

        if (!ptr)
        {
            throw std::bad_alloc();
        }

        return static_cast<T*>(ptr);
    }

    void deallocate(T* ptr, std::size_t n) noexcept
    {
        if (!ptr)
        {
            return;
        }

        MemoryManager::Instance().Deallocate(ptr, n * sizeof(T));
    }

    template<typename U>
    bool operator==(const MMAllocator<U>&) const noexcept
    {
        return true;
    }

    template<typename U>
    bool operator!=(const MMAllocator<U>&) const noexcept
    {
        return false;
    }
};