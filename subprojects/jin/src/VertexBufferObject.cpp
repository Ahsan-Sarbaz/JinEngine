#include "VertexBufferObject.h"
#include "Logger.h"
#include <GL/glew.h>

void VertexBufferObject::Init(VertexBufferObjectType _type)
{
    type = _type;
    glGenBuffers(1, &id);
}

void VertexBufferObject::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBufferObject::SetLayout(VertexBufferObjectLayout* layouts, int count)
{
    layout = layouts;
    layoutCount = count;
}

void VertexBufferObject::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::SetData(u32 size, void* data)
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, type);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::SetSubData(u32 size, u32 offset, void* data)
{
    if(type == VERTEX_BUFFER_OBJECT_TYPE_STATIC_DRAW)
    {
        LOG_ERROR("Cannot set sub data of a static vertex buffer\n");
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}