#include "FixedAllocator.h"
FixedAllocator::FixedAllocator(std::size_t blockSize)
	: blockSize(blockSize)
	, allocChunk(0)
	, deallocChunk(0)
{
	assert(blockSize > 0);

	prev = next = this;

	std::size_t calNumBlocks = DEFAULT_CHUNK_SIZE / blockSize;
	if (calNumBlocks > UCHAR_MAX)
		calNumBlocks = UCHAR_MAX;
	else if (calNumBlocks == 0)
	{
		assert(false);
		//calNumBlocks = 8 * blockSize; //???? bug? calNumBlocks > 255
	}
	numBlocks = static_cast<unsigned char>(calNumBlocks);
	assert(numBlocks == calNumBlocks);
}


FixedAllocator::FixedAllocator(const FixedAllocator& rhs)
	: blockSize(rhs.blockSize)
	, numBlocks(rhs.numBlocks)
	, chunks(rhs.chunks)
{
	prev = &rhs;
	next = rhs.next;
	rhs.next->prev = this;
	rhs.next = this;

	allocChunk = rhs.allocChunk
		? &chunks.front() + (rhs.allocChunk - &rhs.chunks.front())
		: 0;

	deallocChunk = rhs.deallocChunk
		? &chunks.front() + (rhs.deallocChunk - &rhs.chunks.front())
		: 0;
}

//链表不一致问题
FixedAllocator& FixedAllocator::operator=(const FixedAllocator& rhs)
{
	FixedAllocator copy(rhs);
	copy.Swap(*this);
	return *this;
}

FixedAllocator::~FixedAllocator()
{
	if (prev != this)
	{
		prev->next = next;
		next->prev = prev;
		return;
	}

	assert(prev == next);
	Chunks::iterator i = chunks.begin();
	for (; i != chunks.end(); i++)
	{
		assert(i->blocksAvailable == numBlocks);
		i->Release();
	}
}

void FixedAllocator::Swap(FixedAllocator& rhs)
{
	std::swap(blockSize, rhs.blockSize);
	std::swap(numBlocks, rhs.numBlocks);
	chunks.swap(rhs.chunks);
	std::swap(allocChunk, rhs.allocChunk);
	std::swap(deallocChunk, rhs.deallocChunk);
}

void* FixedAllocator::Allocate()
{
	if (allocChunk == 0 || allocChunk->blocksAvailable == 0) 
	{
		Chunks::iterator i = chunks.begin();
		for (;; ++i) 
		{
			// Create a new chunk
			if (i == chunks.end())
			{
				chunks.reserve(chunks.size() + 1);
				Chunk newChunk;
				newChunk.Init(blockSize, numBlocks);
				chunks.push_back(newChunk);
				allocChunk = &chunks.back();
				deallocChunk = &chunks.front();
				break;
			}

			// Try to find an available chunk
			if (i->blocksAvailable > 0)
			{
				allocChunk = &*i;
				break;
			}
		}
	}

	assert(allocChunk != 0);
	assert(allocChunk->blocksAvailable > 0);
	return allocChunk->Allocate(blockSize);
}

void FixedAllocator::Deallocate(void* p)
{
	assert(!chunks.empty());
	assert(&chunks.front() <= deallocChunk);
	assert(&chunks.back() >= deallocChunk);

	deallocChunk = VicinityFind(p);
	assert(deallocChunk);

	DoDeallocate(p);
}

Chunk* FixedAllocator::VicinityFind(void* p)
{
	assert(!chunks.empty());
	assert(deallocChunk);

	// chunk range: [pData, pData + chunkLength)
	const std::size_t chunkLength = numBlocks * blockSize;

	/*
	*   ES:
		chunks[0]  chunks[1]  chunks[2]  chunks[3]  chunks[4]  chunks[5] ... chunks[blocks - 1]
			↑					  ↑			↑                                                      ↑
		  lBound			deallocChunk    |													rBound
								← l         r →													
	*/
	Chunk* l = deallocChunk;
	Chunk* r = deallocChunk + 1;
	Chunk* lBound = &chunks.front();
	Chunk* rBound = &chunks.back() + 1;

	if (r == rBound) r = 0;

	for (;;)
	{
		if (l)
		{
			if (p >= l->pData && p < l->pData + chunkLength)
			{
				return l;
			}
			if (l == lBound) l = 0;
			else --l;
		}

		if (r)
		{
			if (p >= r->pData && p < r->pData + chunkLength)
			{
				return r;
			}
			if (++r == rBound) r = 0;
		}
	}

	assert(false);
	return 0;
}

void FixedAllocator::DoDeallocate(void* p)
{
	/* deallocChunk->pData <= p < deallocChunk->pData + numBlocks * blockSize */
	assert(deallocChunk->pData <= p);
	assert(deallocChunk->pData + numBlocks * blockSize > p);

	deallocChunk->Deallocate(p, blockSize);

	if (deallocChunk->blocksAvailable == numBlocks)
	{
		Chunk& lastChunk = chunks.back();
		// deallocChunk is exactly the last chunk.
		if (&lastChunk == deallocChunk)
		{
			/*
				[ chunk0 ][ chunk1 ][ chunk2 ][ empty ]
											  ↑
											  deallocChunk
											  lastChunk
			*/
			// check if we have two last chunks empty
			if (chunks.size() > 1 &&
				deallocChunk[-1].blocksAvailable == numBlocks)
			{
				/*
					[ chunk0 ][ chunk1 ][ empty ][ empty ]
				*/
				lastChunk.Release();
				chunks.pop_back();
				/*
					[ chunk0 ][ chunk1 ][ empty ]
					↑
					allocChunk???
					deallocChunk
				*/
				allocChunk = deallocChunk = &chunks.front();
			}
			return;
		}

		// deallocChunk is not the last chunk.
		if (lastChunk.blocksAvailable == numBlocks)
		{
			/*
				[ chunk0 ][ empty ][ chunk2 ][ empty ]
						  ↑                  ↑
						  deallocChunk       lastChunk
			*/
			// Two free blocks, discard one
			lastChunk.Release();
			chunks.pop_back();
			/*
				[ chunk0 ][ empty ][ chunk2 ]
						  ↑
						  allocChunk
						  deallocChunk
			*/
			allocChunk = deallocChunk;
		}
		else
		{
			/*
				[ chunk0 ][ empty ][ chunk2 ][ chunk3 ]
						  ↑                  ↑
						  deallocChunk       lastChunk
			*/
			// move the empty chunk to the end
			std::swap(*deallocChunk, lastChunk);
			allocChunk = &chunks.back();
			/*
				[ chunk0 ][ chunk3 ][ chunk2 ][ empty ]
						  ↑                   ↑
						  lastChunk           deallocChunk
											  allocChunk
			*/
		}
	}
}

std::size_t FixedAllocator::GetBlockSize() const
{
	return blockSize;
}