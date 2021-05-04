#include "VertexBufferObject.h"
#include "Memory.h"
#include "Logger.h"
#include <GL/glew.h>

VertexBufferObject* CreateVertexBufferObject(VertexBufferObjectType type)
{
    VertexBufferObject* vbo = (VertexBufferObject*)MemAlloc(sizeof(VertexBufferObject), MEMORY_TAG_STRUCT);
    vbo->type = type;
    glGenBuffers(1, &vbo->id);
    return vbo; 
}

void DeleteVertexBufferObject(VertexBufferObject* vbo)
{
    glDeleteBuffers(1, &vbo->id);
    MemFree(vbo, sizeof(VertexBufferObject), MEMORY_TAG_STRUCT);
}

void BindVertexBufferObject(VertexBufferObject* vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
}

void SetVertexBufferObjectLayout(VertexBufferObject* vbo, VertexBufferObjectLayout* layouts, int count)
{
    vbo->layout = layouts;
    vbo->layoutCount = count;
}

void UnbindVertexBufferObject()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SetVertexBufferObjectData(VertexBufferObject* vbo, u32 size, void* data)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
    glBufferData(GL_ARRAY_BUFFER, size, data, vbo->type);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SetVertexBufferObjectSubData(VertexBufferObject* vbo, u32 size, u32 offset, void* data)
{
    if(vbo->type == VERTEX_BUFFER_OBJECT_TYPE_STATIC_DRAW)
    {
        LOG_ERROR("Cannot set sub data of a static vertex buffer\n");
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}