#include "ShaderProgram.h"
#include "Logger.h"
#include <GL/glew.h>

void ShaderProgram::Init()
{
    id = glCreateProgram();
}

void ShaderProgram::InitFromVFShaderPath(const char* vpath, const char* fpath)
{
    Shader* v_shader = new Shader(SHADER_TYPE_VERTEX);
    Shader* f_shader = new Shader(SHADER_TYPE_FRAGMENT);
    v_shader->SetSourceFromFile(vpath);
    f_shader->SetSourceFromFile(fpath);

    v_shader->Compile();
    f_shader->Compile();
    id =glCreateProgram();
    AttachShader(v_shader);
    AttachShader(f_shader);
    Link();
    delete v_shader;
    delete f_shader;
}

void ShaderProgram::InitFromVFShaderSources(char* vsource, char* fsource)
{
    Shader* v_shader = new Shader(SHADER_TYPE_VERTEX);
    Shader* f_shader = new Shader(SHADER_TYPE_FRAGMENT);
    v_shader->SetSource(&vsource);
    f_shader->SetSource(&fsource);
    v_shader->Compile();
    f_shader->Compile();
    id = glCreateProgram();
    AttachShader(v_shader);
    AttachShader(f_shader);
    Link();
    delete v_shader;
    delete f_shader;
}

// void DeleteShaderProgram(ShaderProgram* program)
// {
//     if(program == nullptr)
//     {
//         LOG_ERROR("Trying to delete a null shader program\n");
//         return;
//     }
//     glDeleteProgram(program->id);
//     MemFree(program, sizeof(ShaderProgram), MEMORY_TAG_STRUCT);
// }

void ShaderProgram::AttachShader(Shader* shader)
{
    glAttachShader(id, shader->GetId());
}

void ShaderProgram::DetachShader(Shader* shader)
{
    glDetachShader(id, shader->GetId());
}

b8 ShaderProgram::Link()
{
    glLinkProgram(id);
    i32 linked = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);
    if(linked != GL_TRUE)
    {   
        i32 info_log_len = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_len);
        char* info_log_buffer = (char*)malloc(sizeof(char) * info_log_len);
        glGetProgramInfoLog(id, info_log_len, 0, info_log_buffer);
        LOG_ERROR("Shader Program : %s\n", info_log_buffer);
        free(info_log_buffer);
        return FALSE;
    }
    return TRUE;
}

void ShaderProgram::Bind()
{
    glUseProgram(id);
}

void ShaderProgram::Unbind()
{
    glUseProgram(0);
}

i32 ShaderProgram::GetUniformLocation(const char* name)
{
    return glGetUniformLocation(id, name);
}

void ShaderProgram::SetUnifromInt(const char* name, i32 x)
{
    auto location = glGetUniformLocation(id, name);
    glUniform1i(location, x);
}
void ShaderProgram::SetUnifromIntArray(const char* name, i32* x, i32 count)
{
    auto location = glGetUniformLocation(id, name);
    glUniform1iv(location, count, x);
}

void ShaderProgram::SetUnifromFloat(const char* name, float x)
{
    auto location = glGetUniformLocation(id, name);
    glUniform1f(location, x);
}

void ShaderProgram::SetUnifromFloat2(const char* name, float x, float y)
{
    auto location = glGetUniformLocation(id, name);
    glUniform2f(location, x, y);
}

void ShaderProgram::SetUnifromFloat3(const char* name, float x, float y, float z)
{
    auto location = glGetUniformLocation(id, name);
    glUniform3f(location, x, y, z);
}

void ShaderProgram::SetUnifromFloat4(const char* name, float x, float y, float z, float w)
{
    auto location = glGetUniformLocation(id, name);
    glUniform4f(location, x, y, z, w);
}

void ShaderProgram::SetUniformVector2(const char* name, const glm::vec2& vec)
{
    auto location = glGetUniformLocation(id, name);
    glUniform2fv(location, 1, glm::value_ptr(vec));
}

void ShaderProgram::SetUniformVector3(const char* name, const glm::vec3& vec)
{
    auto location = glGetUniformLocation(id, name);
    glUniform3fv(location, 1, glm::value_ptr(vec));
}

void ShaderProgram::SetUniformVector4(const char* name, const glm::vec4& vec)
{
    auto location = glGetUniformLocation(id, name);
    glUniform4fv(location, 1, glm::value_ptr(vec));
}
 
void ShaderProgram::SetUniformMatrix4(const char* name, const glm::mat4& mat)
{
    auto location = glGetUniformLocation(id, name);
    glUniformMatrix4fv(location, 1, 0, glm::value_ptr(mat));
}
