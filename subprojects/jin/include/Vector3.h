#pragma once
#include "Types.h"

struct Vector3
{
    union
    {
        struct
        {
            float x, y, z;
        };
        
        struct
        {
            float r, g, b;
        };

        float data[3];
    };
};

typedef Vector3 vec3;
typedef Vector3 v3;

Vector3 Add(const Vector3& a, const Vector3& b);
Vector3 Sub(const Vector3& a, const Vector3& b);
Vector3 Mul(const Vector3& a, const Vector3& b);
Vector3 Div(const Vector3& a, const Vector3& b);
b8 Equals(const Vector3& a, const Vector3& b);
Vector3& AddInPlace(Vector3& a, const Vector3& b);
Vector3& SubInPlace(Vector3& a, const Vector3& b);
Vector3& MulInPlace(Vector3& a, const Vector3& b);
Vector3& DivInPlace(Vector3& a, const Vector3& b);
b8 NotEquals(const Vector3& a, const Vector3& b);