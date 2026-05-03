//#include "AllocatorBenchmark.h"
#include "MemoryFunctions.h"

class A 
{
public:
    A(int a, int b) {
        m_a = a;
        m_b = b;
    }

private:
    int m_a, m_b;
};


int main()
{
    //RunAllBenchmarks();
    
    A* a = MM_NEW(A, 1, 2);
    MM_DELETE(a);

    return 0;
}