#pragma once
#include "Types.h"

struct Vector2
{
    union
    {
        struct
        {
            float x, y;
        };

        float data[2];
    };
};

typedef Vector2 vec2;
typedef Vector2 v2;

Vector2 Add(const Vector2& a, const Vector2& b);
Vector2 Sub(const Vector2& a, const Vector2& b);
Vector2 Mul(const Vector2& a, const Vector2& b);
Vector2 Div(const Vector2& a, const Vector2& b);
b8 Equals(const Vector2& a, const Vector2& b);
Vector2& AddInPlace(Vector2& a, const Vector2& b);
Vector2& SubInPlace(Vector2& a, const Vector2& b);
Vector2& MulInPlace(Vector2& a, const Vector2& b);
Vector2& DivInPlace(Vector2& a, const Vector2& b);
b8 NotEquals(const Vector2& a, const Vector2& b);
