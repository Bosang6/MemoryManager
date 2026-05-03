#include "MemoryManagerGlobal.h"
#include "MemoryManager.h"
#include <iostream>
#ifdef USE_MEMORY_MANAGER_GLOBAL

void* operator new(std::size_t size)
{
    return MemoryManager::Instance().Allocate(size);
}

void operator delete(void* ptr, std::size_t size) noexcept
{
    MemoryManager::Instance().Deallocate(ptr, size);
}

void* operator new[](std::size_t size)
{
    return MemoryManager::Instance().Allocate(size);
}

void operator delete[](void* ptr, std::size_t size) noexcept
{
    MemoryManager::Instance().Deallocate(ptr, size);
}

#endif