#pragma once
#include "Types.h"

enum UniformBufferObjectType
{
    UNIFORM_BUFFER_OBJECT_TYPE_STREAM_DRAW  = 0x88E0,
    UNIFORM_BUFFER_OBJECT_TYPE_STREAM_READ  = 0x88E1,
    UNIFORM_BUFFER_OBJECT_TYPE_STREAM_COPY  = 0x88E2,
    UNIFORM_BUFFER_OBJECT_TYPE_STATIC_DRAW  = 0x88E4,
    UNIFORM_BUFFER_OBJECT_TYPE_STATIC_READ  = 0x88E5,
    UNIFORM_BUFFER_OBJECT_TYPE_STATIC_COPY  = 0x88E6,
    UNIFORM_BUFFER_OBJECT_TYPE_DYNAMIC_DRAW =  0x88E8,
    UNIFORM_BUFFER_OBJECT_TYPE_DYNAMIC_READ =  0x88E9,
    UNIFORM_BUFFER_OBJECT_TYPE_DYNAMIC_COPY =  0x88EA
};

class UniformBufferObject
{
private:
    u32 id;
    UniformBufferObjectType type;

public:
    UniformBufferObject() = default;
    UniformBufferObject(UniformBufferObjectType type);
    void Bind();
    void Unbind();
    void SetData(u32 size, void* data);
    void SetSubData(u32 offset, u32 size, void* data);
    void SetBindingIndex(u32 index);
    inline u32 GetId() { return id; }
    inline UniformBufferObjectType GetType() { return type; }
};