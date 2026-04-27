#include <functional>
#include "SmallObjAllocator.h"

struct CompareFixedAllocatorSize
	: std::binary_function<const FixedAllocator &, std::size_t, bool>
{
	bool operator()(const FixedAllocator& x, std::size_t numBytes) const
	{
		return x.GetBlockSize() < numBytes;
	}
};

SmallObjAllocator::SmallObjAllocator(
		std::size_t chunkSize,
		std::size_t maxObjectSize)
	: pLastAlloc(0)
	, pLastDealloc(0)
	, chunkSize(chunkSize)
	, maxObjectSize(maxObjectSize)
{
}
// 迭代器和lower_bound需要仔细研究一下语法
void* SmallObjAllocator::Allocate(std::size_t numBytes)
{
	if (numBytes > maxObjectSize)
		return operator new(numBytes);

	if (pLastAlloc && pLastAlloc->GetBlockSize() == numBytes)
	{
		return pLastAlloc->Allocate();
	}

	Pool::iterator i = std::lower_bound(pool.begin(), pool.end(), numBytes, CompareFixedAllocatorSize());

	if (i == pool.end() || i->GetBlockSize() != numBytes)
	{
		i = pool.insert(i, FixedAllocator(numBytes));
		pLastDealloc = &*pool.begin();
	}
	pLastAlloc = &*i;
	return pLastAlloc->Allocate();
}

void SmallObjAllocator::Deallocate(void* p, std::size_t numBytes)
{
	if (numBytes > maxObjectSize)
		return operator delete(p);

	if (pLastDealloc && pLastDealloc->GetBlockSize() == numBytes)
	{
		return pLastDealloc->Deallocate(p);
	}

	Pool::iterator i = std::lower_bound(pool.begin(), pool.end(), numBytes, CompareFixedAllocatorSize());

	assert(i != pool.end());
	assert(i->GetBlockSize() == numBytes);
	
	pLastDealloc = &*i;
	pLastDealloc->Deallocate(p);
}