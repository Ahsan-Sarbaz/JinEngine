#pragma once
#include "Types.h"

b8 ReadFileToBuffer(const char* path, char** buffer, int* size);
void FreeFileBuffer(char** buffer, int size);
b8 WriteBufferToFile(const char* path, char** buffer, int size);
b8 AppendBufferToFile(const char* path, char** buffer, int size);