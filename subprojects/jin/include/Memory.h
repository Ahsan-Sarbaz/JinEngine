#pragma once
#include "Types.h"

enum MEMORY_TAG
{
    MEMORY_TAG_MISC,
    MEMORY_TAG_FILE_IO,
    MEMORY_TAG_STRUCT,
    MEMORY_TAG_BATCH_RENDERER_BUFFER,
    MEMORY_TAG_STRING_BUFFER,
    MEMORY_TAG_COUNT
};

struct MemoryState
{
    u64 total_memory;
    u64 memory_consumption;
    u64 memory_tag[MEMORY_TAG_COUNT];
};

void* MemAlloc(u64 size, MEMORY_TAG tag);
void MemFree(void* ptr, u64 size, MEMORY_TAG tag);
void MemoryReport();
const MemoryState* GetMemoryState();
const char* MemoryTagToString(MEMORY_TAG tag);