#pragma once
#include <cstddef>
#include <vector>
#include <string>
#include <iostream>

#include "SmallObject.h"
#include "Timer.h"
#include "SmallObjAllocator.h"

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SMALL_OBJECT_SIZE 64
#endif

template <class T>
void TestClassBulk(const std::string& name, std::size_t count)
{
    std::vector<T*> ptrs;
    ptrs.reserve(count);

    {
        Timer timer(name + " - class bulk allocation");

        for (std::size_t i = 0; i < count; ++i)
        {
            ptrs.push_back(new T());
        }
    }

    {
        Timer timer(name + " - class bulk deallocation");

        for (T* p : ptrs)
        {
            delete p;
        }
    }
}

void TestBulk_System(std::size_t blockSize, std::size_t count);

void TestBulk_SmallObjAllocator(
    SmallObjAllocator& allocator,
    std::size_t blockSize,
    std::size_t count
);

void RunPointerBenchmarks(std::size_t blockSize, std::size_t count);
void RunClassBenchmarks(std::size_t count);
void RunAllBenchmarks();
