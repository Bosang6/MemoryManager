#include "Chunk.h"

void Chunk::Init(std::size_t blockSize, unsigned char blocks) {
	pData = new unsigned char[blockSize * blocks];

	firstAvailableBlock = 0;
	blocksAvailable = blocks;

	unsigned char i = 0;
	unsigned char* p = pData;
	/* 
		 Block 0				       Block 1							   Block n-1
	   | {next free = 1, free space } | {block id = 2, free space } | ... | {block id = blocks, free space } |
	     ^
		 |
		 p

	   The maximum number of blocks is 255. (unsigened char[0 ~ 255])
	*/
	for (; i != blocks; p += blockSize) {
		*p = ++i;
	}
}

void* Chunk::Allocate(std::size_t blockSize) 
{
	if (!blocksAvailable) return 0;

	unsigned char* pResult = pData + (firstAvailableBlock * blockSize);

	//Update firstAvailableBlock
	firstAvailableBlock = *pResult;
	--blocksAvailable;

	return pResult;
}

void Chunk::Deallocate(void* p, std::size_t blockSize)
{
	assert(p >= pData);

	unsigned char* toRelease = static_cast<unsigned char*>(p);

	// Alignment check
	assert((toRelease - pData) % blockSize == 0);

	// Record the index of the next free block
	*toRelease = firstAvailableBlock;

	firstAvailableBlock = static_cast<unsigned char>(
		(toRelease - pData) / blockSize
		);
	// Truncation check
	assert(firstAvailableBlock == (toRelease - pData) / blockSize);

	++blocksAvailable;
}

void Chunk::Release()
{
	delete[] pData;
}

