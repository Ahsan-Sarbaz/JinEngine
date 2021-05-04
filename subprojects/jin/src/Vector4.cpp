#include "Vector4.h"

Vector4 Add(const Vector4& a, const Vector4& b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

Vector4 Sub(const Vector4& a, const Vector4& b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

Vector4 Mul(const Vector4& a, const Vector4& b)
{
    return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

Vector4 Div(const Vector4& a, const Vector4& b)
{
    return {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

b8 Equals(const Vector4& a, const Vector4& b)
{
    return (b8)(a.x == b.x && a.y == b.y && a.z ==  b.z && a.w == b.w);
}

Vector4& AddInPlace(Vector4& a, const Vector4& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    a.w += b.w;
    return a;
}

Vector4& SubInPlace(Vector4& a, const Vector4& b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    a.w -= b.w;
    return a;
}

Vector4& MulInPlace(Vector4& a, const Vector4& b)
{
    a.x *= b.x;
    a.y *= b.y;
    a.z *= b.z;
    a.w *= b.w;
    return a;
}

Vector4& DivInPlace(Vector4& a, const Vector4& b)
{
    a.x /= b.x;
    a.y /= b.y;
    a.z /= b.z;
    a.w /= b.w;
    return a;
}

b8 NotEquals(const Vector4& a, const Vector4& b)
{
    return !Equals(a, b);
}