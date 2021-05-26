#include "IndexBufferObject.h"
 
#include "Logger.h"
#include <GL/glew.h>

void IndexBufferObject::Init(IndexBufferObjectType _type)
{
    type = _type;
    glGenBuffers(1, &id);
}

void IndexBufferObject::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBufferObject::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBufferObject::SetData(u32 size, void* data)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, type);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBufferObject::SetSubData(u32 size, u32 offset, void* data)
{
    if(type == INDEX_BUFFER_OBJECT_TYPE_STATIC_DRAW)
    {
        LOG_ERROR("Cannot set sub data of a static index buffer\n");
        return;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBufferObject::SetCount(u32 count)
{
    indicesCount = count;
}