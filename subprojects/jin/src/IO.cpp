#include "IO.h"
#include "Logger.h"

#include <stdio.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "../include/stb_image.h"
#else
#include "stb_image.h"
#endif

/// FreeFileBuffer the buffer after use
b8 ReadFileToBuffer(const path& fspath, char** buffer, int* size)
{
    std::string path = fspath.string();
    FILE* file = fopen(path.c_str(), "r");
    if(!file)
    {
        LOG_ERROR("Failed to read file %s\n", path.c_str());
        return FALSE;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    *buffer = (char*)malloc(*size);
    rewind(file);
    fread(*buffer, *size, 1, file);
    fclose(file);
    return TRUE;
}

void FreeFileBuffer(char** buffer, int size)
{
    free(*buffer);
}

void FreeTextureBuffer(unsigned char** buffer, int size)
{
    free(*buffer);
}

b8 WriteBufferToFile(const path& fspath, char** buffer, int size)
{
    std::string path = fspath.string();
    FILE* file = fopen(path.c_str(), "w");
    if(!file)
    {
        LOG_ERROR("Failed to write file %s\n", path.c_str());
        return FALSE;
    }

    fwrite(*buffer, 1, size, file);
    fclose(file);
    return TRUE;
}

b8 AppendBufferToFile(const path& fspath, char** buffer, int size)
{
    std::string path = fspath.string();
    FILE* file = fopen(path.c_str(), "a");
    if(!file)
    {
        LOG_ERROR("Failed to append file %s\n", path.c_str());
        return FALSE;
    }

    fwrite(*buffer, 1, size, file);
    fclose(file);
    return TRUE;
}

//TODO:check if stb creates a new buffer internally or not becuase the buffer pointer changes after call to stbi_load_from_memory
b8 ReadTextureToBuffer(const path& fspath, unsigned char** buffer, int* size, int* width, int* height, int* channels)
{
    std::string path = fspath.string();

    *buffer = stbi_load(path.c_str(), width, height, channels, 0);// (*buffer, *size, width, height, channels, 0);
    if (*buffer == NULL)
    {
        LOG_ERROR("Failed to read texture file %s\n", path.c_str());
        return FALSE;
    }
    
    return TRUE;
}