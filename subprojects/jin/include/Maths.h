#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

Vector2 ToVec2(const Vector3& v);
Vector2 ToVec2(const Vector4& v);

Vector3 ToVec3(const Vector2& v); 
Vector3 ToVec3(const Vector4& v);

Vector4 ToVec4(const Vector2& v);
Vector4 ToVec4(const Vector3& v);