#include "MemoryTracker.h"

void MemoryTracker::RegisterAllocation(void* ptr, std::size_t size)
{
	if (!ptr)
	{
		return;
	}

	allocationMap.emplace(ptr, AllocationInfo{ size });

	stats.totalAllocatedBytes += size;
	++stats.allocationCount;
}

void MemoryTracker::RegisterDeallocation(void* ptr, std::size_t size)
{
	if (!ptr)
	{
		return;
	}

	auto it = allocationMap.find(ptr);

	if (it == allocationMap.end())
	{
		std::cout << "[MemoryTracker] Warning: invalid or double free: " << ptr << '\n';
	}
	else
	{
		allocationMap.erase(it);
	}

	stats.totalFreedBytes += size;
	++stats.deallocationCount;
}

void MemoryTracker::PrintStats() const
{
	std::cout << "\n===== Memory Stats =====\n";
	std::cout << "Total allocated: " << stats.totalAllocatedBytes << " bytes\n";
	std::cout << "Total freed    : " << stats.totalFreedBytes << " bytes\n";;
	std::cout << "Alloc count    : " << stats.allocationCount << '\n';
	std::cout << "Free count     : " << stats.deallocationCount << '\n';
}

void MemoryTracker::DumpLeaks() const
{
	std::cout << "\n===== Memory Leak Report =====\n";

	if (allocationMap.empty())
	{
		std::cout << "No memory leaks detected.\n";
		return;
	}

	std::cout << "Memory leaks detected:\n";
	std::size_t totalLeak = 0;
	for (const auto& pair : allocationMap)
	{
		std::cout << "Leaked ptr: " << pair.first
			      << ", size: " << pair.second.size
			      << " bytes\n";

		totalLeak += pair.second.size;
	}
	std::cout << "Memory total leaks: " << totalLeak << std::endl;
}

const MemoryStats& MemoryTracker::GetStats() const noexcept
{
	return stats;
}