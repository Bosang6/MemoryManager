#pragma once

#include <cstddef>
#include <map>
#include <iostream>

#include "InternalAllocator.h"

struct MemoryStats
{
    std::size_t totalAllocatedBytes = 0;
    std::size_t totalFreedBytes = 0;

    std::size_t allocationCount = 0;
    std::size_t deallocationCount = 0;
};

struct AllocationInfo
{
    std::size_t size = 0;
};

class MemoryTracker
{
public:
    void RegisterAllocation(void* ptr, std::size_t size);
    void RegisterDeallocation(void* ptr, std::size_t size);

    void PrintStats() const;
    void DumpLeaks() const;

    const MemoryStats& GetStats() const noexcept;

private:
    using AllocationPair = std::pair<void* const, AllocationInfo>;

    using AllocationMap = std::map<
        void*,
        AllocationInfo,
        std::less<void*>,
        InternalAllocator<AllocationPair>
    >;

private:
    AllocationMap allocationMap;
    MemoryStats stats;
};