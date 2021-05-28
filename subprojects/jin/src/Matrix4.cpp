#include "Matrix4.h"
#include <string.h>

Matrix4::Matrix4()
{
    col1 = {};
    col2 = {};
    col3 = {};
    col4 = {};
}

Matrix4::Matrix4(const vec4& a, const vec4& b, const vec4& c, const vec4& d)
{
    col1 = a;
    col2 = b;
    col3 = c;
    col4 = d;
}

Matrix4::Matrix4(const glm::mat4& glmMat4)
{
    data[0][0] = glmMat4[0][0];    
    data[0][1] = glmMat4[0][1];    
    data[0][2] = glmMat4[0][2];    
    data[0][3] = glmMat4[0][3];    
    data[1][0] = glmMat4[1][0];    
    data[1][1] = glmMat4[1][1];    
    data[1][2] = glmMat4[1][2];    
    data[1][3] = glmMat4[1][3];    
    data[2][0] = glmMat4[2][0];    
    data[2][1] = glmMat4[2][1];    
    data[2][2] = glmMat4[2][2];    
    data[2][3] = glmMat4[2][3];    
    data[3][0] = glmMat4[3][0];    
    data[3][1] = glmMat4[3][1];    
    data[3][2] = glmMat4[3][2];    
    data[3][3] = glmMat4[3][3];
}

Matrix4 CreateIdentityMatrix4()
{
    return Matrix4{
        {1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1},
    };
}

Matrix4 Mult(const Matrix4& a, const Matrix4& b)
{
    float data[16];
    for (i32 row = 0; row < 4; row++)
    {
        for (i32 col = 0; col < 4; col++)
        {
            float sum = 0.0f;
            for (i32 e = 0; e < 4; e++)
            {
                sum += a.elements[e + row * 4] * b.elements[col + e * 4];
            }
            data[col + row * 4] = sum;
        }
    }
    Matrix4 result;
    memcpy(result.data, data, 4 * 4 * sizeof(float));
    return result;
}

Matrix4 CreateOrthographicProjectionMatrix4(float left, float right, float bottom ,float top, float near, float far)
{
    Matrix4 result = CreateIdentityMatrix4();
    result.data[0][0] = 2 / (right - left);
    result.data[1][1] = 2 / (top - bottom);
    result.data[2][2] = - 2 / (far - near);
    result.data[3][0] = - (right + left) / (right - left);
    result.data[3][1] = - (top + bottom) / (top - bottom);
    result.data[3][2] = - (far + near) / (far - near);
    return result;
}

Matrix4 CreatePerspectiveProjectionMatrix4(float fov, float width, float height, float near, float far)
{
    const float rad = fov;
    const float h = cosf(0.5f * rad) / sinf(0.5f * rad);
    const float w = h * height / width;

    Matrix4 result;
    result.data[0][0] = w;
    result.data[1][1] = h;
    result.data[2][2] = - (far + near) / (far - near);
    result.data[2][3] = - 1;
    result.data[3][2] = - (2 * far * near) / (far - near);
    return result;
}

Matrix4 CreateLookAtMatrix4(const vec3& eye, const vec3& center, const vec3& up)
{
    const vec3 f(Normalize(Sub(center, eye)));
    const vec3 s(Normalize(Cross(f, up)));
    const vec3 u(Cross(s, f));

    mat4 result = CreateIdentityMatrix4();
    result.data[0][0] = s.x;
    result.data[1][0] = s.y;
    result.data[2][0] = s.z;
    result.data[0][1] = u.x;
    result.data[1][1] = u.y;
    result.data[2][1] = u.z;
    result.data[0][2] =-f.x;
    result.data[1][2] =-f.y;
    result.data[2][2] =-f.z;
    result.data[3][0] =-Dot(s, eye);
    result.data[3][1] =-Dot(u, eye);
    result.data[3][2] = Dot(f, eye);
    return result;

}