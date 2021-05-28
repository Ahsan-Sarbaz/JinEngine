#include "Vector3.h"
#include "math.h"

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

vec3 Cross(const Vector3& a, const Vector3& b)
{
    return vec3{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

float Dot(const Vector3& a, const Vector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Magnitude(const Vector3& a)
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

vec3 Normalize(const Vector3& a)
{
    float len = Magnitude(a);
    return vec3{a.x / len, a.y / len, a.z / len};
}

float Distance(const Vector3& a, const Vector3& b)
{
    float x = a.x - b.x;
    float y = a.y - b.y;
    float z = a.z - b.z;
    return sqrt(x * x + y * y + z * z);
}