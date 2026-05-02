#pragma once
#include <cstddef>

class GeneralAllocator
{
public:
	void* Allocate(std::size_t size)
	{
		return ::operator new(size);
	}

	void Deallocate(void* ptr)
	{
		::operator delete(ptr);
	}
};