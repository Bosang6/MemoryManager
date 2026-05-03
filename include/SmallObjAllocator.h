#pragma once
#include <cstddef>
#include <vector>
#include "FixedAllocator.h"
#include "InternalAllocator.h"
class SmallObjAllocator
{
public:
	SmallObjAllocator(
		std::size_t chunkSize,
		std::size_t maxObjectSize);
	void* Allocate(std::size_t numBytes);
	void Deallocate(void* p, std::size_t size);
private:
	SmallObjAllocator(const SmallObjAllocator&);
	SmallObjAllocator& operator=(const SmallObjAllocator&);

	typedef std::vector<FixedAllocator, InternalAllocator<FixedAllocator>> Pool;
	Pool pool;
	FixedAllocator* pLastAlloc;
	FixedAllocator* pLastDealloc;
protected:
	std::size_t chunkSize;
	std::size_t maxObjectSize;
};