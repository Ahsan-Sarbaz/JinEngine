#include "ShaderProgram.h"
#include "Memory.h"
#include "Logger.h"
#include <GL/glew.h>

ShaderProgram* CreateShaderProgram()
{
    ShaderProgram* program = (ShaderProgram*)MemAlloc(sizeof(ShaderProgram), MEMORY_TAG_STRUCT);
    program->id = glCreateProgram();
    return program;
}

ShaderProgram* CreateShaderProgramFromVFShaderPath(const char* vpath, const char* fpath)
{
    Shader* v_shader = CreateShader(SHADER_TYPE_VERTEX);
    Shader* f_shader = CreateShader(SHADER_TYPE_FRAGMENT);
    SetShaderSourceFromFile(v_shader, vpath);
    SetShaderSourceFromFile(f_shader, fpath);
    CompileShader(v_shader);
    CompileShader(f_shader);
    ShaderProgram* program = CreateShaderProgram();
    AttachShaderToShaderProgram(program, v_shader);
    AttachShaderToShaderProgram(program, f_shader);
    LinkShaderProgram(program);
    DeleteShader(v_shader);
    DeleteShader(f_shader);
    return program;
}

ShaderProgram* CreateShaderProgramFromVFShaderSources(char* vsource, char* fsource)
{
    Shader* v_shader = CreateShader(SHADER_TYPE_VERTEX);
    Shader* f_shader = CreateShader(SHADER_TYPE_FRAGMENT);
    SetShaderSource(v_shader, &vsource);
    SetShaderSource(f_shader, &fsource);
    CompileShader(v_shader);
    CompileShader(f_shader);
    ShaderProgram* program = CreateShaderProgram();
    AttachShaderToShaderProgram(program, v_shader);
    AttachShaderToShaderProgram(program, f_shader);
    LinkShaderProgram(program);
    DeleteShader(v_shader);
    DeleteShader(f_shader);
    return program;
}

void DeleteShaderProgram(ShaderProgram* program)
{
    if(program == nullptr)
    {
        LOG_ERROR("Trying to delete a null shader program\n");
        return;
    }
    glDeleteProgram(program->id);
    MemFree(program, sizeof(ShaderProgram), MEMORY_TAG_STRUCT);
}

void AttachShaderToShaderProgram(ShaderProgram* program, Shader* shader)
{
    glAttachShader(program->id, shader->id);
}

void DetachShaderFromShaderProgram(ShaderProgram* program, Shader* shader)
{
    glDetachShader(program->id, shader->id);
}

b8 LinkShaderProgram(ShaderProgram* program)
{
    glLinkProgram(program->id);
    i32 linked = 0;
    glGetProgramiv(program->id, GL_LINK_STATUS, &linked);
    if(linked != GL_TRUE)
    {   
        i32 info_log_len = 0;
        glGetProgramiv(program->id, GL_INFO_LOG_LENGTH, &info_log_len);
        char* info_log_buffer = (char*)MemAlloc(sizeof(char) * info_log_len, MEMORY_TAG_STRING_BUFFER);
        glGetProgramInfoLog(program->id, info_log_len, 0, info_log_buffer);
        LOG_ERROR("Shader Program : %s\n", info_log_buffer);
        MemFree(info_log_buffer, sizeof(char) * info_log_len, MEMORY_TAG_STRING_BUFFER);
        return FALSE;
    }
    return TRUE;
}

void UseShaderProgram(ShaderProgram* program)
{
    glUseProgram(program->id);
}

i32 GetUniformLocation(ShaderProgram* program, const char* name)
{
    return glGetUniformLocation(program->id, name);
}

void SetUnifromInt(ShaderProgram* program, const char* name, i32 x)
{
    auto location = glGetUniformLocation(program->id, name);
    glUniform1i(location, x);
}
void SetUnifromIntArray(ShaderProgram* program, const char* name, i32* x, i32 count)
{
    auto location = glGetUniformLocation(program->id, name);
    glUniform1iv(location, count, x);
}

void SetUnifromFloat(ShaderProgram* program, const char* name, float x)
{
    auto location = glGetUniformLocation(program->id, name);
    glUniform1f(location, x);
}

void SetUnifromFloat2(ShaderProgram* program, const char* name, float x, float y)
{
    auto location = glGetUniformLocation(program->id, name);
    glUniform2f(location, x, y);
}

void SetUnifromFloat3(ShaderProgram* program, const char* name, float x, float y, float z)
{
    auto location = glGetUniformLocation(program->id, name);
    glUniform3f(location, x, y, z);
}

void SetUnifromFloat4(ShaderProgram* program, const char* name, float x, float y, float z, float w)
{
    auto location = glGetUniformLocation(program->id, name);
    glUniform4f(location, x, y, z, w);
}

void SetUniformVector2(ShaderProgram* program, const char* name, const Vector2& vec)
{
    auto location = glGetUniformLocation(program->id, name);
    glUniform2fv(location, 1, vec.data);
}

void SetUniformVector3(ShaderProgram* program, const char* name, const Vector3& vec)
{
    auto location = glGetUniformLocation(program->id, name);
    glUniform3fv(location, 1, vec.data);
}

void SetUniformVector4(ShaderProgram* program, const char* name, const Vector4& vec)
{
    auto location = glGetUniformLocation(program->id, name);
    glUniform4fv(location, 1, vec.data);
}

void SetUniformMatrix4(ShaderProgram* program, const char* name, const Matrix4& mat)
{
    auto location = glGetUniformLocation(program->id, name);
    glUniformMatrix4fv(location, 1, 0, *mat.data);
}
