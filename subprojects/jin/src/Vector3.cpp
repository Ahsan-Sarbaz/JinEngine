#include "Vector3.h"



Vector3 Add(const Vector3& a, const Vector3& b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z };
}

Vector3 Sub(const Vector3& a, const Vector3& b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z };
}

Vector3 Mul(const Vector3& a, const Vector3& b)
{
    return {a.x * b.x, a.y * b.y, a.z * b.z };
}

Vector3 Div(const Vector3& a, const Vector3& b)
{
    return {a.x / b.x, a.y / b.y, a.z / b.z };
}

b8 Equals(const Vector3& a, const Vector3& b)
{
    return (b8)(a.x == b.x && a.y == b.y && a.z ==  b.z);
}

Vector3& AddInPlace(Vector3& a, const Vector3& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

Vector3& SubInPlace(Vector3& a, const Vector3& b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}

Vector3& MulInPlace(Vector3& a, const Vector3& b)
{
    a.x *= b.x;
    a.y *= b.y;
    a.z *= b.z;
    return a;
}

Vector3& DivInPlace(Vector3& a, const Vector3& b)
{
    a.x /= b.x;
    a.y /= b.y;
    a.z /= b.z;
    return a;
}

b8 NotEquals(const Vector3& a, const Vector3& b)
{
    return !Equals(a, b);
}