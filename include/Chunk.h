#pragma once
#include <cstddef>
#include <cassert>

struct Chunk
{
	void Init(std::size_t blockSize, unsigned char blocks);
	void* Allocate(std::size_t blockSize);
	void Deallocate(void* p, std::size_t blockSize);
	void Release();
	unsigned char* pData;
	unsigned char
		firstAvailableBlock,
		blocksAvailable;
};