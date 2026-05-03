#pragma once
#include <cstdlib>
template<typename T>
class InternalAllocator
{
public:
    using value_type = T;

    T* allocate(std::size_t n)
    {
        void* ptr = std::malloc(n * sizeof(T));
        if (!ptr)
        {
            throw std::bad_alloc();
        }
        return static_cast<T*>(ptr);
    }

    void deallocate(T* ptr, std::size_t)
    {
        std::free(ptr);
    }
};