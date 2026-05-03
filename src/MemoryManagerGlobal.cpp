#include "MemoryManagerGlobal.h"
#include "MemoryManager.h"
#include <iostream>
#ifdef USE_MEMORY_MANAGER_GLOBAL

void* operator new(std::size_t size)
{
    std::puts("using overridden global operator new");
    return MemoryManager::Instance().Allocate(size);
}

void operator delete(void* ptr, std::size_t size) noexcept
{
    std::cout << "using overrided global operator delete" << std::endl;
    MemoryManager::Instance().Deallocate(ptr, size);
}

void* operator new[](std::size_t size)
{
    std::cout << "using overrided global operator new[]" << std::endl;
    return MemoryManager::Instance().Allocate(size);
}

void operator delete[](void* ptr, std::size_t size) noexcept
{
    std::cout << "using overrided global operator delete[]" << std::endl;
    MemoryManager::Instance().Deallocate(ptr, size);
}

#endif