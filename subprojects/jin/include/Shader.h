#pragma once
#include "Types.h"

enum ShaderType
{
    SHADER_TYPE_FRAGMENT = 0x8B30,
    SHADER_TYPE_VERTEX = 0x8B31,
    // TODO: support these and more shaders
    SHADER_TYPE_GEOMETRY = 0x8DD9,
};

struct Shader
{
    u32 id;
    ShaderType type;
};

Shader* CreateShader(ShaderType type);
void DeleteShader(Shader* shader);
void SetShaderSource(Shader* shader, char** buffer);
b8 CompileShader(Shader* shader);
const char* ShaderTypeToString(ShaderType type);
b8 SetShaderSourceFromFile(Shader* shader, const char* path);