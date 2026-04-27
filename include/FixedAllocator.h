#pragma once
#include <vector>
#include "Chunk.h"

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

class FixedAllocator
{
public:
	explicit FixedAllocator(std::size_t blockSize = 0);
	FixedAllocator(const FixedAllocator&);
	FixedAllocator& operator=(const FixedAllocator&);
	~FixedAllocator();

	void* Allocate();
	void Deallocate(void* p);
	std::size_t GetBlockSize() const;

private:
	Chunk* VicinityFind(void* p);
	void DoDeallocate(void* p);
	void Swap(FixedAllocator& rhs);
	std::size_t blockSize;
	unsigned char numBlocks;
	typedef std::vector<Chunk> Chunks;
	Chunks chunks;
	Chunk* allocChunk;
	Chunk* deallocChunk;

	// For ensuring proper copy semantics
	mutable const FixedAllocator* prev;
	mutable const FixedAllocator* next;
};