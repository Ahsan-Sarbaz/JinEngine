#include "Vector2.h"
#include "math.h"

Vector2 Add(const Vector2& a, const Vector2& b)
{
    return {a.x + b.x, a.y + b.y };
}

Vector2 Sub(const Vector2& a, const Vector2& b)
{
    return {a.x - b.x, a.y - b.y };
}

Vector2 Mul(const Vector2& a, const Vector2& b)
{
    return {a.x * b.x, a.y * b.y };
}

Vector2 Div(const Vector2& a, const Vector2& b)
{
    return {a.x / b.x, a.y / b.y };
}

b8 Equals(const Vector2& a, const Vector2& b)
{
    return (b8)(a.x == b.x && a.y == b.y);
}

Vector2& AddInPlace(Vector2& a, const Vector2& b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

Vector2& SubInPlace(Vector2& a, const Vector2& b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

Vector2& MulInPlace(Vector2& a, const Vector2& b)
{
    a.x *= b.x;
    a.y *= b.y;
    return a;
}

Vector2& DivInPlace(Vector2& a, const Vector2& b)
{
    a.x /= b.x;
    a.y /= b.y;
    return a;
}

b8 NotEquals(const Vector2& a, const Vector2& b)
{
    return !Equals(a, b);
}

float Dot(const Vector2& a, const Vector2& b)
{
    return a.x * b.x + a.y * b.y; 
}

float Magnitude(const Vector2& a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

Vector2 Normalize(const Vector2& a)
{
    float len = Magnitude(a);
    return vec2{a.x / len, a.y / len};
}

float Distance(const Vector2& a, const Vector2& b)
{
    float x = a.x - b.x;
    float y = a.y - b.y;
    return sqrt(x * x + y * y);
}