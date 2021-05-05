#pragma once
#include "Types.h"

b8 ReadFileToBuffer(const char* path, char** buffer, int* size);
b8 ReadTextureToBuffer(const char* path, unsigned char** buffer, int* size, int* width, int* height, int* channels);
void FreeFileBuffer(char** buffer, int size);
void FreeTextureBuffer(unsigned char** buffer, int size);
b8 WriteBufferToFile(const char* path, char** buffer, int size);
b8 AppendBufferToFile(const char* path, char** buffer, int size);