#pragma once
#include <cstddef>

#include "MemoryManager.h"

#ifdef USE_MEMORY_MANAGER_GLOBAL

void* operator new(std::size_t size);
void operator delete(void* ptr, std::size_t size) noexcept;

void* operator new[](std::size_t size);
void operator delete[](void* ptr, std::size_t size) noexcept;

#endif
