#pragma once
#include "Types.h"

struct RectI
{
    union
    {
        struct
        {
            i32 x, y, w, h;
        };
        i32 data[4];
    };
};

struct RectF
{
    union
    {
        struct
        {
            float x, y, w, h;
        };
        float data[4];
    };
};

typedef RectI irect;
typedef RectF frect;