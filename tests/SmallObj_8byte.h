#pragma once
class SmallObj_8byte
{
public:
    SmallObj_8byte()
        :m_a(10), m_b(20)
    {
    }
    SmallObj_8byte(int a, int b) {
        m_a = a;
        m_b = b;
    }

private:
    int m_a, m_b;
};