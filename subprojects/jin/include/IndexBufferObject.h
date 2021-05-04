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

struct IndexBufferObject
{
    u32 id;
    IndexBufferObjectType type;
    u32 indicesCount;
};

typedef IndexBufferObject IBO;

IndexBufferObject* CreateIndexBufferObject(IndexBufferObjectType type);
void DeleteIndexBufferObject(IndexBufferObject* ibo);
void BindIndexBufferObject(IndexBufferObject* ibo);
void UnbindIndexBufferObject();
void SetIndexBufferObjectData(IndexBufferObject* ibo, u32 size, void* data);
void SetIndexBufferObjectSubData(IndexBufferObject* ibo, u32 size, u32 offset, void* data);
void SetIndexBufferIndicesCount(IndexBufferObject* ibo, u32 count);