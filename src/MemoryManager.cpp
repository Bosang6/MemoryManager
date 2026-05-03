#include "MemoryManager.h"

MemoryManager& MemoryManager::Instance()
{
	static MemoryManager instance;
	return instance;
}

MemoryManager::MemoryManager() 
	: smallObjAllocator(DEFAULT_CHUNK_SIZE, MAX_SMALL_OBJECT_SIZE)
{
}

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