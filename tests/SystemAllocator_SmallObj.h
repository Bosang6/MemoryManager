#pragma once
struct SystemAllocator_SmallObj
{
    int id;
    float x;
    float y;
    float z;
    float vx;
    float vy;
    float vz;

    SystemAllocator_SmallObj()
        : id(0), x(0), y(0), z(0), vx(1), vy(1), vz(0)
    {
    }
};