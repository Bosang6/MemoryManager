#include "MemoryManagerGlobal.h"
#include "MemoryManager.h"
#include <iostream>
#ifdef USE_MEMORY_MANAGER_GLOBAL

#pragma region Sized Version

#ifdef MM_USE_SIZED_DELETE
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
#pragma endregion


#pragma region Unsized Version
#else
struct AllocationHeader
{
    std::size_t totalSize;
};

void* operator new(std::size_t size)
{
    std::size_t totalSize = sizeof(AllocationHeader) + size;

    void* totalPlace = MemoryManager::Instance().Allocate(totalSize);

    AllocationHeader* header = static_cast<AllocationHeader*>(totalPlace);
    header->totalSize = totalSize;

    void* userPlace = header + 1;

    return userPlace;
}

void operator delete(void* ptr) noexcept
{
    if(!ptr)
    {
        return;
    }

    AllocationHeader* header = static_cast<AllocationHeader*>(ptr) - 1;
    MemoryManager::Instance().Deallocate(header, header->totalSize);
}


void* operator new[](std::size_t size)
{
    std::size_t totalSize = sizeof(AllocationHeader) + size;

    void* totalPlace = MemoryManager::Instance().Allocate(totalSize);

    AllocationHeader* header = static_cast<AllocationHeader*>(totalPlace);
    header->totalSize = totalSize;

    void* userPlace = header + 1;

    return userPlace;
}

void operator delete[](void* ptr) noexcept
{
    if (!ptr)
    {
        return;
    }

    AllocationHeader* header = static_cast<AllocationHeader*>(ptr) - 1;
    MemoryManager::Instance().Deallocate(header, header->totalSize);
}
#endif

#pragma endregion

#endif