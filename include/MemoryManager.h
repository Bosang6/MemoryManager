#pragma once
#include <cstddef>
#include "SmallObjAllocator.h"
#include "GeneralAllocator.h"
#include "MemoryTracker.h"

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SMALL_OBJECT_SIZE 64
#endif

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

class MemoryManager
{
public:
	static MemoryManager& Instance();
	void* Allocate(std::size_t size);
	void Deallocate(void* ptr, std::size_t size);

private:
	MemoryManager();
	~MemoryManager();
	MemoryManager(const MemoryManager&) = delete;
	MemoryManager& operator=(const MemoryManager&) = delete;

private:
	SmallObjAllocator smallObjAllocator;
	GeneralAllocator generalAllocator;
	MemoryTracker tracker;
};
