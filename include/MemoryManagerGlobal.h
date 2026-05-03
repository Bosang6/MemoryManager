#pragma once
#include <cstddef>

#include "MemoryManager.h"

#if defined(__cpp_sized_deallocation) || defined(_MSC_VER)
#define MM_USE_SIZED_DELETE
#endif

#ifdef USE_MEMORY_MANAGER_GLOBAL

void* operator new(std::size_t size);
void* operator new[](std::size_t size);

#if defined(MM_USE_SIZED_DELETE)
void operator delete(void* ptr, std::size_t size) noexcept;
void operator delete[](void* ptr, std::size_t size) noexcept;
#endif

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

#endif
