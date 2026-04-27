#include "AllocatorBenchmark.h"
#include "SmallAllocator_SmallObj.h"
#include "SystemAllocator_SmallObj.h"

void TestBulk_System(std::size_t blockSize, std::size_t count)
{
    std::vector<void*> ptrs;
    ptrs.reserve(count);

    {
        Timer timer("System - Bulk allocation");

        for (std::size_t i = 0; i < count; ++i)
        {
            ptrs.push_back(::operator new(blockSize));
        }
    }

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

    {
        Timer timer("SmallObjAllocator - Bulk allocation");

        for (std::size_t i = 0; i < count; ++i)
        {
            ptrs.push_back(allocator.Allocate(blockSize));
        }
    }

    {
        Timer timer("SmallObjAllocator - Bulk deallocation");

        for (void* p : ptrs)
        {
            allocator.Deallocate(p, blockSize);
        }
    }
}

void RunPointerBenchmarks(std::size_t blockSize, std::size_t count)
{
    std::cout << "===== Bulk pointer allocation =====\n";

    TestBulk_System(blockSize, count);

    SmallObjAllocator allocator{
        DEFAULT_CHUNK_SIZE,
        MAX_SMALL_OBJECT_SIZE
    };

    TestBulk_SmallObjAllocator(allocator, blockSize, count);

    std::cout << '\n';
}

void RunClassBenchmarks(std::size_t count)
{
    std::cout << "===== Class Bulk =====\n";

    std::cout << "sizeof(SystemAllocator_SmallObj): "
        << sizeof(SystemAllocator_SmallObj) << '\n';

    std::cout << "sizeof(SmallAllocator_SmallObj): "
        << sizeof(SmallAllocator_SmallObj) << '\n';

    TestClassBulk<SystemAllocator_SmallObj>("SystemAllocator_SmallObj", count);
    TestClassBulk<SmallAllocator_SmallObj>("SmallAllocator_SmallObj", count);

    std::cout << '\n';
}

void RunAllBenchmarks()
{
    const std::size_t blockSize = 32;
    const std::size_t count = 1'000'000;

    RunPointerBenchmarks(blockSize, count);
    RunClassBenchmarks(count);
}