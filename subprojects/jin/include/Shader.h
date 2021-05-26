#pragma once
#include "Types.h"

enum ShaderType
{
    SHADER_TYPE_FRAGMENT = 0x8B30,
    SHADER_TYPE_VERTEX = 0x8B31,
    // TODO: support these and more shaders
    SHADER_TYPE_GEOMETRY = 0x8DD9,
};

class Shader
{
    private:
    u32 id;
    ShaderType type;
    
    public:
    Shader() = default;
    void Init(ShaderType type);
    void SetSource(char** buffer);
    b8 Compile();
    b8 SetSourceFromFile(const char* path);
    inline u32 GetId() { return id; }
    static const char* TypeToString(ShaderType type);
};