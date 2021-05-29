#include "VertexArrayObject.h"
#include "Logger.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include <GL/glew.h>

void VertexArrayObject::Init()
{
    glGenVertexArrays(1, &id);
}

void VertexArrayObject::PushVertexBuffer(VertexBufferObject* vbo)
{
    Bind();
    vbo->Bind();
    for (u32 i = 0; i < vbo->GetLayoutCount(); ++i)
    {
        VertexBufferObjectLayout layout = vbo->GetLayout(i);
        glVertexAttribPointer(layout.index, layout.size, layout.type, layout.normalized, layout.stride, layout.pointer);
        glEnableVertexAttribArray(layout.index);
    }
    Unbind();
}

void VertexArrayObject::PushIndexBuffer(IndexBufferObject* ibo)
{
    Bind();
    ibo->Bind();
    Unbind();
}

void VertexArrayObject::Bind()
{
    glBindVertexArray(id);    
}

void VertexArrayObject::Unbind()
{
    glBindVertexArray(0);
}