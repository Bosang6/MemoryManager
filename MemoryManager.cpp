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
/*--------------------- Bulk pointer --------------------------------------*/
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
/*--------------------- Bulk pointer --------------------------------------*/
/*--------------------- Bulk object --------------------------------------*/
struct SystemBullet
{
    int id;
    float x;
    float y;
    float vx;
    float vy;
    int damage;
    int life;
    int type;

    SystemBullet()
        : id(0), x(0), y(0), vx(1), vy(1), damage(10), life(100), type(0)
    {
    }
};
struct SmallBullet : public SmallObject<>
{
    int id;
    float x;
    float y;
    float vx;
    float vy;
    int damage;
    int life;
    int type;

    SmallBullet()
        : id(0), x(0), y(0), vx(1), vy(1), damage(10), life(100), type(0)
    {
    }
};

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

/*--------------------- Bulk object --------------------------------------*/
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

    {
        std::cout << "===== Class Bulk =====\n";
        TestClassBulk<SystemBullet>("SystemBullet", count);
        TestClassBulk<SmallBullet>("SmallBullet", count);
        std::cout << '\n';
    }
    
    return 0;
}