#include "MemoryManager.h"

void* MemoryManager::Allocate(std::size_t size)
{
	if (size < SMALL_OBJ_MAX_SIZE) 
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

	if (size <= SMALL_OBJ_MAX_SIZE)
	{
		smallObjAllocator.Deallocate(ptr, size);
	}
	else
	{
		generalAllocator.Deallocate(ptr);
	}
}