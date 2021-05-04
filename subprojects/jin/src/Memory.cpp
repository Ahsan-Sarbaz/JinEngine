#include "Memory.h"
#include "Logger.h"

#include <stdlib.h>
#include <cstring>

static MemoryState memory_state;

void* MemAlloc(u64 size, MEMORY_TAG tag)
{
    memory_state.total_memory += size;
    memory_state.memory_tag[tag] += size;
    memory_state.memory_consumption += size;

    void* ptr = malloc(size);
    memset(ptr, 0, size);
    return ptr;
}

void MemFree(void* ptr, u64 size, MEMORY_TAG tag)
{
    if(ptr)
    {
        free(ptr);
        memory_state.total_memory -= size;
        memory_state.memory_tag[tag] -= size;
        ptr = nullptr;
    }
    else
    {
        LOG_ERROR("Cannot Free a nullptr!\n");
    }
}

void MemoryReport()
{
    LOG_INFO("%lld B %.2f KB %.2f MB\n", memory_state.total_memory, (double)memory_state.total_memory / 1024.0, (double)memory_state.total_memory / (1024.0 * 1024.0));
}

const MemoryState* GetMemoryState()
{
    return &memory_state;
}

const char* MemoryTagToString(MEMORY_TAG tag)
{
    switch (tag)
    {
        case MEMORY_TAG_MISC:
            return "MISC";
        break;
        case MEMORY_TAG_STRUCT:
            return "STRUCTS";
        break;
        case MEMORY_TAG_FILE_IO:
            return "FILE_IO";
        break;
        case MEMORY_TAG_STRING_BUFFER:
            return "STRING BUFFER";
        break;
        case MEMORY_TAG_BATCH_RENDERER_BUFFER:
            return "BATCH RENDERER BUFFER";
        break;
        //just here for the warning
        case MEMORY_TAG_COUNT:
        break;
    }

    return "UNKNOWN";
}