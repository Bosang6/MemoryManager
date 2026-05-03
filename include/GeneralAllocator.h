#pragma once
#include <cstddef>
#include <cstdlib>
#include <iostream>
class GeneralAllocator
{
public:
	void* Allocate(std::size_t size)
	{
#ifdef USE_MEMORY_MANAGER_GLOBAL
		//::operator new by default use malloc
		return std::malloc(size);
#else
		return ::operator new(size);
#endif
	}

	void Deallocate(void* ptr)
	{
#ifdef USE_MEMORY_MANAGER_GLOBAL
		return std::free(ptr);
#else
		::operator delete(ptr);
#endif
	}
};