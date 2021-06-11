#pragma once

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef i32 b32;
typedef u8 b8;

#define TRUE 1
#define FALSE 0

#define GLM_FORCE_SSE2
#define GLM_FORCE_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define M_PI 3.141592
#endif