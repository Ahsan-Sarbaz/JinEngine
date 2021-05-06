#pragma once
#include "Types.h"
#include "Shader.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

struct ShaderProgram
{
    u32 id;   
};

ShaderProgram* CreateShaderProgram();
/// this will read the two files compile them and link them to a progam and delete them because they are now in gpu memory
ShaderProgram* CreateShaderProgramFromVFShaderPath(const char* vpath, const char* fpath);
/// this will create shaders from sources compile them and link them to a progam and delete them because they are now in gpu memory
ShaderProgram* CreateShaderProgramFromVFShaderSources(char* vsource, char* fsource);
void DeleteShaderProgram(ShaderProgram* program);
void AttachShaderToShaderProgram(ShaderProgram* program, Shader* shader);
void DetachShaderFromShaderProgram(ShaderProgram* program, Shader* shader);
b8 LinkShaderProgram(ShaderProgram* program);
void UseShaderProgram(ShaderProgram* program);
i32 GetUniformLocation(ShaderProgram* program, const char* name);
//TODO: use uniform location caching
void SetUnifromInt(ShaderProgram* program, const char* name, i32 x);
void SetUnifromIntArray(ShaderProgram* program, const char* name, i32* x, i32 count);
void SetUnifromFloat(ShaderProgram* program, const char* name, float x);
void SetUnifromFloat2(ShaderProgram* program, const char* name, float x, float y);
void SetUnifromFloat3(ShaderProgram* program, const char* name, float x, float y, float z);
void SetUnifromFloat4(ShaderProgram* program, const char* name, float x, float y, float z, float w);
void SetUniformVector2(ShaderProgram* program, const char* name, const Vector2& vec);
void SetUniformVector3(ShaderProgram* program, const char* name, const Vector3& vec);
void SetUniformVector4(ShaderProgram* program, const char* name, const Vector4& vec);
void SetUniformMatrix4(ShaderProgram* program, const char* name, const Matrix4& mat);