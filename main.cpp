//#include "AllocatorBenchmark.h"
#include "MemoryFunctions.h"
#include <iostream>

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

private:
    int m_a, m_b;
};


int main()
{
    //RunAllBenchmarks();

    void* p = MM_MALLOC(10);
    MM_FREE(p, 10);
    
    A* a = MM_NEW(A, 1, 2);
    MM_DELETE(a);

    A* array = MM_NEW_A(A, 10);
    MM_DELETE_A(array, 10);

    return 0;
}