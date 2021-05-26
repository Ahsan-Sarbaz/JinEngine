#pragma once
#include "Types.h"

enum VertexBufferObjectLayoutType
{
    //TODO: add more types like r g b a luminance etc
    VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_BYTE = 0x1400,
    VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_UNSIGNED_BYTE = 0x1401,
    VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_SHORT = 0x1402,
    VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_UNSIGNED_SHORT = 0x1403,
    VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_INT = 0x1404,
    VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_UNSIGNED_INT = 0x1405,
    VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT = 0x1406
};

struct VertexBufferObjectLayout
{
    u32 index;
    i32 size;
    VertexBufferObjectLayoutType type;
    b8 normalized;
    i32 stride;
    void* pointer;
};

enum VertexBufferObjectType
{
    VERTEX_BUFFER_OBJECT_TYPE_STREAM_DRAW  = 0x88E0,
    VERTEX_BUFFER_OBJECT_TYPE_STREAM_READ  = 0x88E1,
    VERTEX_BUFFER_OBJECT_TYPE_STREAM_COPY  = 0x88E2,
    VERTEX_BUFFER_OBJECT_TYPE_STATIC_DRAW  = 0x88E4,
    VERTEX_BUFFER_OBJECT_TYPE_STATIC_READ  = 0x88E5,
    VERTEX_BUFFER_OBJECT_TYPE_STATIC_COPY  = 0x88E6,
    VERTEX_BUFFER_OBJECT_TYPE_DYNAMIC_DRAW =  0x88E8,
    VERTEX_BUFFER_OBJECT_TYPE_DYNAMIC_READ =  0x88E9,
    VERTEX_BUFFER_OBJECT_TYPE_DYNAMIC_COPY =  0x88EA
};

class VertexBufferObject
{
private:
    u32 id;
    VertexBufferObjectLayout* layout;
    u32 layoutCount;
    VertexBufferObjectType type;

public:
    VertexBufferObject() = default;
    void Init(VertexBufferObjectType type);
    void Bind();
    void Unbind();
    void SetLayout(VertexBufferObjectLayout* layouts, int count);
    void SetData(u32 size, void* data);
    /// call SetData(vbo, size, nullptr) before calling this data=nullptr
    void SetSubData(u32 size, u32 offset, void* data);
    u32 GetLayoutCount() { return layoutCount;};
    VertexBufferObjectLayout GetLayout(u32 index) { return layout[index]; }
};

typedef VertexBufferObject VBO;
typedef VertexBufferObjectLayout VBOLayout;
