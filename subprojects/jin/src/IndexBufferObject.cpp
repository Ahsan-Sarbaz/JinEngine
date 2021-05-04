#include "IndexBufferObject.h"
#include "Memory.h"
#include "Logger.h"
#include <GL/glew.h>

IndexBufferObject* CreateIndexBufferObject(IndexBufferObjectType type)
{
    IndexBufferObject* ibo = (IndexBufferObject*)MemAlloc(sizeof(IndexBufferObject), MEMORY_TAG_STRUCT);
    ibo->type = type;
    glGenBuffers(1, &ibo ->id);
    return ibo ; 
}

void DeleteIndexBufferObject(IndexBufferObject* ibo )
{
    glDeleteBuffers(1, &ibo ->id);
    MemFree(ibo , sizeof(IndexBufferObject), MEMORY_TAG_STRUCT);
}

void BindIndexBufferObject(IndexBufferObject* ibo )
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id);
}

void UnbindIndexBufferObject()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SetIndexBufferObjectData(IndexBufferObject* ibo, u32 size, void* data)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, ibo->type);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SetIndexBufferObjectSubData(IndexBufferObject* ibo, u32 size, u32 offset, void* data)
{
    if(ibo->type == INDEX_BUFFER_OBJECT_TYPE_STATIC_DRAW)
    {
        LOG_ERROR("Cannot set sub data of a static index buffer\n");
        return;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SetIndexBufferIndicesCount(IndexBufferObject* ibo, u32 count)
{
    ibo->indicesCount = count;
}