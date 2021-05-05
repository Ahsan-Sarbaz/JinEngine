#include "IO.h"
#include "Logger.h"
#include "Memory.h"
#include <stdio.h>
#include "stb_image.h"

/// FreeFileBuffer the buffer after use
b8 ReadFileToBuffer(const char* path, char** buffer, int* size)
{
    FILE* file = fopen(path, "r");
    if(!file)
    {
        LOG_ERROR("Failed to read file %s\n", path);
        return FALSE;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    *buffer = (char*)MemAlloc(*size, MEMORY_TAG_FILE_IO);
    rewind(file);
    fread(*buffer, *size, 1, file);
    fclose(file);
    return TRUE;
}

void FreeFileBuffer(char** buffer, int size)
{
    MemFree(*buffer, size, MEMORY_TAG_FILE_IO);
}

void FreeTextureBuffer(unsigned char** buffer, int size)
{
    MemFree(*buffer, size, MEMORY_TAG_TEXTURE_BUFFER);
}

b8 WriteBufferToFile(const char* path, char** buffer, int size)
{
    FILE* file = fopen(path, "w");
    if(!file)
    {
        LOG_ERROR("Failed to write file %s\n", path);
        return FALSE;
    }

    fwrite(*buffer, 1, size, file);
    fclose(file);
    return TRUE;
}

b8 AppendBufferToFile(const char* path, char** buffer, int size)
{
    FILE* file = fopen(path, "a");
    if(!file)
    {
        LOG_ERROR("Failed to append file %s\n", path);
        return FALSE;
    }

    fwrite(*buffer, 1, size, file);
    fclose(file);
    return TRUE;
}

//TODO:check if stb creates a new buffer internally or not becuase the buffer pointer changes after call to stbi_load_from_memory
b8 ReadTextureToBuffer(const char* path, unsigned char** buffer, int* size, int* width, int* height, int* channels)
{
    FILE* file = fopen(path, "r");
    if(!file)
    {
        LOG_ERROR("Failed to read texture file %s\n", path);
        return FALSE;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    *buffer = (unsigned char*)MemAlloc(*size, MEMORY_TAG_TEXTURE_BUFFER);
    rewind(file);
    fread(*buffer, *size, 1, file);
    fclose(file);

    *buffer = stbi_load_from_memory(*buffer, *size, width, height, channels, 0);
    if(*buffer == NULL)
        return FALSE;
    
    return TRUE;
}