#include "Shader.h"
#include "Logger.h"
#include "IO.h"
#include <GL/glew.h>
#include <stdlib.h>

Shader::Shader(ShaderType _type)
    :type(_type)
{
    id = glCreateShader(type);
}

void Shader::SetSource(char** buffer)
{
    glShaderSource(id, 1, buffer, 0);
}

b8 Shader::Compile()
{
    glCompileShader(id);
    i32 compiled = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
    if(compiled != GL_TRUE)
    {   
        i32 info_log_len = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_len);
        char* info_log_buffer = (char*)malloc(sizeof(char) * info_log_len);
        glGetShaderInfoLog(id, info_log_len, 0, info_log_buffer);
        LOG_ERROR("%s: %s\n", Shader::TypeToString(type), info_log_buffer);
        free(info_log_buffer);
        return FALSE;
    }
    return TRUE;
}

const char* Shader::TypeToString(ShaderType type)
{
    switch(type)
    {
        case SHADER_TYPE_VERTEX:
            return "VERTEX SHADER"; 
        break;
        case SHADER_TYPE_FRAGMENT: 
            return "FRAGMENT SHADER"; 
        break;
        case SHADER_TYPE_GEOMETRY: 
            return "GEOMETRY SHADER"; 
        break;
    }
}

b8 Shader::SetSourceFromFile(const char* path)
{
    char* buffer = 0;
    int size = 0;
    b8 res = ReadFileToBuffer(path, &buffer, &size);
    if(!res) return res;
    Shader::SetSource(&buffer);
    FreeFileBuffer(&buffer, size);
    return res;
}