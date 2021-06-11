#pragma once
#include "Types.h"
#include "Shader.h"

class ShaderProgram
{
private:
    u32 id;
public:
    ShaderProgram() = default;
    void Init();
    /// this will read the two files compile them and link them to a progam and delete them because they are now in gpu memory
    void InitFromVFShaderPath(const char* vpath, const char* fpath);
    /// this will create shaders from sources compile them and link them to a progam and delete them because they are now in gpu memory
    void InitFromVFShaderSources(const char* vsource, const char* fsource);
    void AttachShader(Shader* shader);
    void DetachShader(Shader* shader);
    b8 Link();
    void Bind();
    void Unbind();
    i32 GetUniformLocation(const char* name);
    //TODO: use uniform location caching
    void SetUnifromInt(const char* name, i32 x);
    void SetUnifromIntArray(const char* name, i32* x, i32 count);
    void SetUnifromFloat(const char* name, float x);
    void SetUnifromFloat2(const char* name, float x, float y);
    void SetUnifromFloat3(const char* name, float x, float y, float z);
    void SetUnifromFloat4(const char* name, float x, float y, float z, float w);
    void SetUniformVector2(const char* name, const glm::vec2& vec);
    void SetUniformVector3(const char* name, const glm::vec3& vec);
    void SetUniformVector4(const char* name, const glm::vec4& vec);
    void SetUniformMatrix4(const char* name, const glm::mat4& mat);

    inline u32 GetId() { return id; }  
};

