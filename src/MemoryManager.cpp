#include "MemoryManager.h"

void* MemoryManager::Allocate(std::size_t size)
{
	if (size < MAX_SMALL_OBJECT_SIZE)
	{
		return smallObjAllocator.Allocate(size);
	}
	else
	{
		return generalAllocator.Allocate(size);
	}
}

void MemoryManager::Deallocate(void* ptr, std::size_t size)
{
	if (!ptr) return;

	if (size <= MAX_SMALL_OBJECT_SIZE)
	{
		smallObjAllocator.Deallocate(ptr, size);
	}
	else
	{
		generalAllocator.Deallocate(ptr);
	}
}