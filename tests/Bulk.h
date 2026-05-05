#pragma once
#include <cstddef>
#include <vector>
#include "Timer.h"
#include "MemoryFunctions.h"

#ifdef USE_MEMORY_MANAGER_GLOBAL
template<typename T>
void TestBulk_System(std::size_t count)
{
	std::vector<T*> ptrs;
	ptrs.reserve(count);

	{
		Timer timer("System [Global Overload] - Bulk allocation");

		for (std::size_t i = 0; i < count; ++i)
		{
			ptrs.push_back(new T());
		}
	}

	{
		Timer timer("System [Global Overload] - Bulk deallocation");

		for (std::size_t i = 0; i < count; ++i)
		{
			delete ptrs[i];
		}
	}
}
#else
template<typename T>
void TestBulk_System(std::size_t count)
{
	std::vector<T*> ptrs;
	ptrs.reserve(count);

	{
		Timer timer("System - Bulk allocation");

		for (std::size_t i = 0; i < count; ++i)
		{
			ptrs.push_back(new T());
		}
	}

	{
		Timer timer("System - Bulk deallocation");

		for (std::size_t i = 0; i < count; ++i)
		{
			delete ptrs[i];
		}
	}
}
#endif
template<typename T>
void TestBulk_MM(std::size_t count)
{
	std::vector<T*> ptrs;
	ptrs.reserve(count);

	{
		Timer timer("SmallObjAllocator - Bulk allocation");

		for (std::size_t i = 0; i < count; ++i)
		{
			ptrs.push_back(MM_NEW(T));
		}
	}

	{
		Timer timer("SmallObjAllocator - Bulk deallocation");

		for (std::size_t i = 0; i < count; ++i)
		{
			MM_DELETE(ptrs[i]);
		}
	}
}
