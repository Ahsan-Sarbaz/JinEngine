#include "Vector4.h"
#include "math.h"

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

float Dot(const Vector4& a, const Vector4& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float Magnitude(const Vector4& a)
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

Vector4 Normalize(const Vector4& a)
{
    float len = Magnitude(a);
    return Vector4{a.x / len, a.y / len, a.z / len, a.w / len};
}

float Distance(const Vector4& a, const Vector4& b)
{
    float x = a.x - b.x;
    float y = a.y - b.y;
    float z = a.z - b.z;
    float w = a.w - b.w;
    return sqrt(x * x + y * y + z * z + w * w);
}