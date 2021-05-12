#include "Maths.h"

Vector2 ToVec2(const Vector3& v) { return {v.x, v.y}; }
Vector2 ToVec2(const Vector4& v) { return {v.x, v.y}; }

Vector3 ToVec3(const Vector2& v) { return {v.x, v.y, 0}; }
Vector3 ToVec3(const Vector4& v) { return {v.x, v.y, v.z}; }

Vector4 ToVec4(const Vector2& v) { return {v.x, v.y, 0, 0}; }
Vector4 ToVec4(const Vector3& v) { return {v.x, v.y, v.z, 0}; }