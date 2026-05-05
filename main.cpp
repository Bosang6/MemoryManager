#include <iostream>
#include "MemoryManagerGlobal.h"
#include "MemoryFunctions.h"
#include "MMAllocator.h"
#include "Bulk.h"
#include "SmallObj_8byte.h"
#include "SmallObj_32byte.h"
int main()
{

#pragma region Custom Allocator in STL
    //std::vector<A, MMAllocator<A>> vec;

    //vec.emplace_back(1, 2);
    //vec.emplace_back(3, 4);
    //vec.emplace_back(5, 6);

#pragma endregion

    //TestBulk_SameOrder_System<SmallObj_8byte>(1000000);
    //TestBulk_SameOrder_MM<SmallObj_8byte>(1000000);

    //TestBulk_ReverseOrder_System<SmallObj_8byte>(1000000);
    //TestBulk_ReverseOrder_MM<SmallObj_8byte>(1000000);

    

    TestBulk_SameOrder_System<SmallObj_32byte>(1000000);
    TestBulk_SameOrder_MM<SmallObj_32byte>(1000000);

    TestBulk_ReverseOrder_System<SmallObj_32byte>(1000000);
    TestBulk_ReverseOrder_MM<SmallObj_32byte>(1000000);

    return 0;
}