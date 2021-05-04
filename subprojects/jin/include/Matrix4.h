#pragma once
#include <Vector4.h>
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
    };

    Matrix4();
    Matrix4(const vec4& a, const vec4& b, const vec4& c, const vec4& d);
    Matrix4(const glm::mat4& glmMat4);
};

typedef Matrix4 mat4;
typedef Matrix4 m4;

Matrix4 CreateIdentityMatrix4();
Matrix4 CreateOrthographicProjectionMatrix4(float left, float right, float bottom ,float top, float near, float far);
//TODO: test these
Matrix4 CreatePerspectiveProjectionMatrix4(float fov, float width, float height, float near, float far);