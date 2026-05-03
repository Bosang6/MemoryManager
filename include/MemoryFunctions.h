#pragma once

#include <cstddef>
#include "MemoryManager.h"

#define MM_MALLOC(size)    MM_Malloc(size)
#define MM_FREE(ptr, size) MM_Free(ptr, size)
#define MM_NEW(T, ...)     MM_New<T>(__VA_ARGS__)
#define MM_DELETE(ptr)     MM_Delete(ptr)

#pragma region C style Functions

void* MM_Malloc(std::size_t size)
{
	return MemoryManager::Instance().Allocate(size);
}

void MM_Free(void* ptr, std::size_t size)
{
	MemoryManager::Instance().Deallocate(ptr, size);
}

#pragma endregion

#pragma region C++ Style Functions
template<typename T, typename... Args>
T* MM_New(Args&&... args)
{
	void* place = MemoryManager::Instance().Allocate(sizeof(T));

	if (place)
	{
		try
		{
			// placement new
			return new(place) T(std::forward<Args>(args)...);
		}
		catch (...)
		{
			MemoryManager::Instance().Deallocate(place, sizeof(T));
			throw;
		}
	}

	return nullptr;
}

template<typename T>
void MM_Delete(T* ptr)
{
	if (!ptr)
	{
		return;
	}

	ptr->~T();
	MemoryManager::Instance().Deallocate(ptr, sizeof(T));
}
#pragma endregion
