#include <iostream>
#include <vector>
#include "timer.h"
#include "SmallObject.h"

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SMALL_OBJECT_SIZE 64
#endif

void TestBulk_System(std::size_t blockSize, std::size_t count)
{
    std::vector<void*> ptrs;
    ptrs.reserve(count);

    // allocate test
    {
        Timer timer("System - Bulk allocation");

        for (std::size_t i = 0; i < count; ++i)
        {
            ptrs.push_back(::operator new(blockSize));
        }
    }

    // deallocate test 
    {
        Timer timer("System - Bulk deallocation");

        for (void* p : ptrs)
        {
            ::operator delete(p);
        }
    }
}

void TestBulk_SmallObjAllocator(
    SmallObjAllocator& allocator,
    std::size_t blockSize,
    std::size_t count)
{
    std::vector<void*> ptrs;
    ptrs.reserve(count);

    // allocate test
    {
        Timer timer("SmallObjAllocator - Bulk allocation");

        for (std::size_t i = 0; i < count; ++i)
        {
            ptrs.push_back(allocator.Allocate(blockSize));
        }
    }

    // deallocate test 
    {
        Timer timer("SmallObjAllocator - Bulk deallocation");

        for (void* p : ptrs)
        {
            allocator.Deallocate(p, blockSize);
        }
    }
}

int main()
{
    const std::size_t blockSize = 32;
    const std::size_t count = 1000000;

    {
        std::cout << "===== Bulk allocation =====\n";
        TestBulk_System(blockSize, count);

        SmallObjAllocator allocator{ DEFAULT_CHUNK_SIZE , MAX_SMALL_OBJECT_SIZE };
        TestBulk_SmallObjAllocator(allocator, blockSize, count);
    }
    
    return 0;
}