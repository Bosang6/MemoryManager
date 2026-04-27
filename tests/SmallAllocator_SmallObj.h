#pragma once
#include "SmallObject.h"
struct SmallAllocator_SmallObj : public SmallObject<>
{
    int id;
    float x;
    float y;
    float z;
    float vx;
    float vy;
    float vz;

    SmallAllocator_SmallObj()
        : id(0), x(0), y(0), z(0), vx(1), vy(1), vz(0)
    {
    }
};