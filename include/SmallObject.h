#pragma once
#include <cstddef>
#include "SmallObjAllocator.h"
#include "SingletonHolder.h"
#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SMALL_OBJECT_SIZE 64
#endif

template
<
    std::size_t chunkSize = DEFAULT_CHUNK_SIZE,
    std::size_t maxSmallObjectSize = MAX_SMALL_OBJECT_SIZE
>
class SmallObject
{
public:
    static void* operator new(std::size_t size)
    {
        return SingletonHolder<MySmallObjAllocator>::Instance().Allocate(size);
    }

    static void operator delete(void* p, std::size_t size)
    {
        SingletonHolder<MySmallObjAllocator>::Instance().Deallocate(p, size);
    }

    virtual ~SmallObject() {}

private:
    struct MySmallObjAllocator : public SmallObjAllocator
    {
        MySmallObjAllocator()
            : SmallObjAllocator(chunkSize, maxSmallObjectSize)
        {
        }
    };
};