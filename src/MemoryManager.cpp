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

MemoryManager::~MemoryManager()
{
	tracker.DumpLeaks();
	tracker.PrintStats();
}

void* MemoryManager::Allocate(std::size_t size)
{
	void* ptr = nullptr;
	if (size < MAX_SMALL_OBJECT_SIZE)
	{
		ptr = smallObjAllocator.Allocate(size);
	}
	else
	{
		ptr = generalAllocator.Allocate(size);
	}

	if (ptr)
	{
		tracker.RegisterAllocation(ptr, size);
	}

	return ptr;
}

void MemoryManager::Deallocate(void* ptr, std::size_t size)
{
	if (!ptr) return;

	tracker.RegisterDeallocation(ptr, size);

	if (size <= MAX_SMALL_OBJECT_SIZE)
	{
		smallObjAllocator.Deallocate(ptr, size);
	}
	else
	{
		generalAllocator.Deallocate(ptr);
	}
}