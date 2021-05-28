#include "UniformBufferObject.h"
#include <GL/glew.h>

UniformBufferObject::UniformBufferObject(UniformBufferObjectType _type)
    :type(_type)
{
    glGenBuffers(1, &id);
}

void UniformBufferObject::Bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, id);
}

void UniformBufferObject::SetData(u32 size, void* data)
{
    glBindBuffer(GL_UNIFORM_BUFFER, id);
    glBufferData(GL_UNIFORM_BUFFER, size, data, type);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::SetSubData(u32 offset, u32 size, void* data)
{
    glBindBuffer(GL_UNIFORM_BUFFER, id);
    glBufferSubData(GL_UNIFORM_BUFFER,offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::Unbind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::SetBindingIndex(u32 index)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, id);
    // glBindBufferRange(GL_UNIFORM_BUFFER, index, id, 0, sizeof(float) * 16 * 2);
}