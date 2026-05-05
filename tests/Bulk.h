#pragma once
#include <cstddef>
#include <vector>
#include "Timer.h"
#include "MemoryFunctions.h"

#ifdef USE_MEMORY_MANAGER_GLOBAL
template<typename T>
void TestBulk_SameOrder_System(std::size_t count)
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
		Timer timer("System [Global Overload] - SameOrder deallocation");

		for (std::size_t i = 0; i < count; ++i)
		{
			delete ptrs[i];
		}
	}
}

template<typename T>
void TestBulk_ReverseOrder_System(std::size_t count)
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
		Timer timer("System [Global Overload] - ReverseOrder deallocation");

		for (std::size_t i = count; i > 0; --i)
		{
			delete ptrs[i - 1];
		}
	}
}

#else
template<typename T>
void TestBulk_SameOrder_System(std::size_t count)
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
		Timer timer("System - SameOrder deallocation");

		for (std::size_t i = 0; i < count; ++i)
		{
			delete ptrs[i];
		}
	}
}

template<typename T>
void TestBulk_ReverseOrder_System(std::size_t count)
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
		Timer timer("System - ReverseOrder deallocation");

		for (std::size_t i = count; i > 0; --i)
		{
			delete ptrs[i - 1];
		}
	}
}
#endif
template<typename T>
void TestBulk_SameOrder_MM(std::size_t count)
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
		Timer timer("SmallObjAllocator - SameOrder deallocation");

		for (std::size_t i = 0; i < count; ++i)
		{
			MM_DELETE(ptrs[i]);
		}
	}
}

template<typename T>
void TestBulk_ReverseOrder_MM(std::size_t count)
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
		Timer timer("SmallObjAllocator - ReverseOrder deallocation");

		for (std::size_t i = count; i > 0; --i)
		{
			MM_DELETE(ptrs[i - 1]);
		}
	}
}
