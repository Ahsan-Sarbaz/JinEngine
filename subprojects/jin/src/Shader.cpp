#include "Shader.h"
#include "Memory.h"
#include "Logger.h"
#include "IO.h"
#include <GL/glew.h>

Shader* CreateShader(ShaderType type)
{
    Shader* shader = (Shader*)MemAlloc(sizeof(Shader), MEMORY_TAG_STRUCT);
    shader->id = glCreateShader(type);
    return shader;
}

void DeleteShader(Shader* shader)
{
    if(shader == nullptr)
    {
        LOG_ERROR("Trying to delete a null shader\n");
        return;
    }
    glDeleteShader(shader->id);
    MemFree(shader, sizeof(shader), MEMORY_TAG_STRUCT);
}

void SetShaderSource(Shader* shader, char** buffer)
{
    glShaderSource(shader->id, 1, buffer, 0);
}

b8 CompileShader(Shader* shader)
{
    glCompileShader(shader->id);
    i32 compiled = 0;
    glGetShaderiv(shader->id, GL_COMPILE_STATUS, &compiled);
    if(compiled != GL_TRUE)
    {   
        i32 info_log_len = 0;
        glGetShaderiv(shader->id, GL_INFO_LOG_LENGTH, &info_log_len);
        char* info_log_buffer = (char*)MemAlloc(sizeof(char) * info_log_len, MEMORY_TAG_STRING_BUFFER);
        glGetShaderInfoLog(shader->id, info_log_len, 0, info_log_buffer);
        LOG_ERROR("%s: %s\n",ShaderTypeToString(shader->type), info_log_buffer);
        MemFree(info_log_buffer, sizeof(char) * info_log_len, MEMORY_TAG_STRING_BUFFER);
        return FALSE;
    }
    return TRUE;
}

const char* ShaderTypeToString(ShaderType type)
{
    switch(type)
    {
        case SHADER_TYPE_VERTEX: return "VERTEX SHADER"; break;
        case SHADER_TYPE_FRAGMENT: return "FRAGMENT SHADER"; break;
        case SHADER_TYPE_GEOMETRY: return "GEOMETRY SHADER"; break;
    }

    return "UNKNOWN SHADER TYPE";
}

b8 SetShaderSourceFromFile(Shader* shader, const char* path)
{

    char* buffer = 0;
    int size = 0;
    b8 res = ReadFileToBuffer(path, &buffer, &size);
    if(!res) return res;
    SetShaderSource(shader, &buffer);
    FreeFileBuffer(&buffer, size);
    return res;
}