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
#ifdef MM_ENABLE_TRACKING
	tracker.DumpLeaks();
	tracker.PrintStats();
#endif
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

#ifdef MM_ENABLE_TRACKING
	if (ptr)
	{
		tracker.RegisterAllocation(ptr, size);
	}
#endif
	return ptr;
}

void MemoryManager::Deallocate(void* ptr, std::size_t size)
{
	if (!ptr) return;

#ifdef MM_ENABLE_TRACKING
	tracker.RegisterDeallocation(ptr, size);
#endif

	if (size <= MAX_SMALL_OBJECT_SIZE)
	{
		smallObjAllocator.Deallocate(ptr, size);
	}
	else
	{
		generalAllocator.Deallocate(ptr);
	}
}