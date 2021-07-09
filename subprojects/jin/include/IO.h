#pragma once
#include "Types.h"

b8 ReadFileToBuffer(const path& path, char** buffer, int* size);
b8 ReadTextureToBuffer(const path& path, unsigned char** buffer, int* size, int* width, int* height, int* channels);
void FreeFileBuffer(char** buffer, int size);
void FreeTextureBuffer(unsigned char** buffer, int size);
b8 WriteBufferToFile(const path& path, char** buffer, int size);
b8 AppendBufferToFile(const path& path, char** buffer, int size);