//#include "AllocatorBenchmark.h"
#include "MemoryFunctions.h"
#include <iostream>
#include "MemoryManagerGlobal.h"
#include "MMAllocator.h"
class A 
{
public:
    A()
        :m_a(10), m_b(20)
    { }
    A(int a, int b) {
        m_a = a;
        m_b = b;
    }
    ~A()
    {
        std::cout << "m_a = " << m_a << ", m_b = " << m_b << std::endl;
    }
    void Print() const
    {
        std::cout << "A(" << m_a << ", " << m_b << ")\n";
    }

private:
    int m_a, m_b;
};

int main()
{
    //RunAllBenchmarks();

#pragma region Functions Test
    //void* p = MM_MALLOC(10);
    //MM_FREE(p, 10);
    //
    //A* a = MM_NEW(A, 1, 2);
    //MM_DELETE(a);

    //A* array = MM_NEW_A(A, 10);
    //MM_DELETE_A(array, 10);
#pragma endregion

#pragma region Global Override Test
    A* a = new A();
    delete(a);

    A* arrayGeneral = new A[10]; // 10 * 8(sizeof(A)) + 8(array cookie) = 88 > 64 => GeneralAllocator
    delete[] arrayGeneral;

    A* array = new A[5]; // (5 * 8(sizeof(A)) + 8(array cookie) = 48 < 64 => SmallObjAllocator
    delete[] array;
#pragma endregion

#pragma region Custom Allocator in STL
    //std::vector<A, MMAllocator<A>> vec;

    //vec.emplace_back(1, 2);
    //vec.emplace_back(3, 4);
    //vec.emplace_back(5, 6);

#pragma endregion

    return 0;
}