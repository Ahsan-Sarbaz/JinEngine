#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include <glm/glm.hpp>

struct Matrix4
{
    union
    {
        struct
        {
            vec4 col1;
            vec4 col2;
            vec4 col3;
            vec4 col4;
        };
        
        vec4 cols[4];
        float data[4][4];
        float elements[16];
    };

    Matrix4();
    Matrix4(const vec4& a, const vec4& b, const vec4& c, const vec4& d);
    Matrix4(const glm::mat4& glmMat4);
};

typedef Matrix4 mat4;
typedef Matrix4 m4;

Matrix4 Mult(const Matrix4& a, const Matrix4& b);

Matrix4 CreateIdentityMatrix4();
Matrix4 CreateOrthographicProjectionMatrix4(float left, float right, float bottom ,float top, float near, float far);
Matrix4 CreatePerspectiveProjectionMatrix4(float fov, float width, float height, float near, float far);
Matrix4 CreateLookAtMatrix4(const vec3& eye, const vec3& center, const vec3& up);