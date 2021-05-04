#include "IO.h"
#include "Logger.h"
#include "Memory.h"
#include <stdio.h>

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