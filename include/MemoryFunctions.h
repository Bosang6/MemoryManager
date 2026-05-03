#pragma once

#include <cstddef>
#include "MemoryManager.h"

#define MM_MALLOC(size)			MM_Malloc(size)
#define MM_FREE(ptr, size)		MM_Free(ptr, size)
#define MM_NEW(T, ...)		    MM_New<T>(__VA_ARGS__)
#define MM_NEW_A(T, count)		MM_NewArray<T>(count)
#define MM_DELETE(ptr)          MM_Delete(ptr)
#define MM_DELETE_A(ptr, count) MM_DeleteArray(ptr,count)

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
T* MM_NewArray(std::size_t count)
{
	if (count == 0)
	{
		return nullptr;
	}

	void* place = MemoryManager::Instance().Allocate(sizeof(T) * count);
	T* array = static_cast<T*>(place);

	std::size_t constructed = 0;

	try
	{
		for (; constructed < count; ++constructed)
		{
			//placement new for each element at right position
			new(&array[constructed]) T();
		}
	}
	catch (...)
	{
		for (std::size_t i = constructed; i > 0; --i)
		{
			array[i - 1].~T();
		}

		MemoryManager::Instance().Deallocate(place, sizeof(T) * count);
		throw;
	}

	return array;
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

template<typename T>
void MM_DeleteArray(T* ptr, std::size_t count)
{
	if (!ptr)
	{
		return;
	}

	for (std::size_t i = count; i > 0; --i)
	{
		ptr[i - 1].~T();
	}

	MemoryManager::Instance().Deallocate(ptr, sizeof(T) * count);
}
#pragma endregion
