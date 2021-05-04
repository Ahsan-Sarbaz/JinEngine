#pragma once
#include "Types.h"

struct Vector4
{
    union
    {
        struct
        {
            float x, y, z, w;
        };
        
        struct
        {
            float r, g, b, a;
        };

        float data[4];
    };
};

typedef Vector4 vec4;
typedef Vector4 v4;

Vector4 Add(const Vector4& a, const Vector4& b);
Vector4 Sub(const Vector4& a, const Vector4& b);
Vector4 Mul(const Vector4& a, const Vector4& b);
Vector4 Div(const Vector4& a, const Vector4& b);
b8 Equals(const Vector4& a, const Vector4& b);
Vector4& AddInPlace(Vector4& a, const Vector4& b);
Vector4& SubInPlace(Vector4& a, const Vector4& b);
Vector4& MulInPlace(Vector4& a, const Vector4& b);
Vector4& DivInPlace(Vector4& a, const Vector4& b);
b8 NotEquals(const Vector4& a, const Vector4& b);
