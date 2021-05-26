#pragma once
#include "Types.h"


enum IndexBufferObjectType
{
    INDEX_BUFFER_OBJECT_TYPE_STREAM_DRAW  = 0x88E0,
    INDEX_BUFFER_OBJECT_TYPE_STREAM_READ  = 0x88E1,
    INDEX_BUFFER_OBJECT_TYPE_STREAM_COPY  = 0x88E2,
    INDEX_BUFFER_OBJECT_TYPE_STATIC_DRAW  = 0x88E4,
    INDEX_BUFFER_OBJECT_TYPE_STATIC_READ  = 0x88E5,
    INDEX_BUFFER_OBJECT_TYPE_STATIC_COPY  = 0x88E6,
    INDEX_BUFFER_OBJECT_TYPE_DYNAMIC_DRAW =  0x88E8,
    INDEX_BUFFER_OBJECT_TYPE_DYNAMIC_READ =  0x88E9,
    INDEX_BUFFER_OBJECT_TYPE_DYNAMIC_COPY =  0x88EA
};

class IndexBufferObject
{
private:
    u32 id;
    IndexBufferObjectType type;
    u32 indicesCount;
public:
    IndexBufferObject() = default;
    void Init(IndexBufferObjectType type);
    void Bind();
    void Unbind();
    void SetData(u32 size, void* data);
    void SetSubData(u32 size, u32 offset, void* data);
    void SetCount(u32 count);
    
    inline u32 GetCount() { return indicesCount; }
};

typedef IndexBufferObject IBO;