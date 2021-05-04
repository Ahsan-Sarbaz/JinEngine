#include "Matrix4.h"

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