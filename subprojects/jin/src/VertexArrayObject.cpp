#include "VertexArrayObject.h"
#include "Memory.h"
#include "Logger.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include <GL/glew.h>

VertexArrayObject* CreateVertexArrayObject()
{
    VertexArrayObject* vao = (VertexArrayObject*)MemAlloc(sizeof(VertexArrayObject), MEMORY_TAG_STRUCT);
    glGenVertexArrays(1, &vao->id);
    return vao;
}

void DeleteVertexArrayobject(VertexArrayObject* vao)
{
    glDeleteVertexArrays(1, &vao->id);
    MemFree(vao, sizeof(VertexArrayObject), MEMORY_TAG_STRUCT);
}

void PushVertexBufferIntoVertexArrayObject(VertexArrayObject* vao, VertexBufferObject* vbo)
{
    BindVertexArrayObject(vao);
    BindVertexBufferObject(vbo);
    for (u32 i = 0; i < vbo->layoutCount; i++)
    {
        VertexBufferObjectLayout layout = vbo->layout[i];
        glVertexAttribPointer(layout.index, layout.size, layout.type, layout.normalized, layout.stride, layout.pointer);
        glEnableVertexAttribArray(layout.index);
    }
}

void PushIndexBufferIntoVertexArrayObject(VertexArrayObject* vao, IndexBufferObject* ibo)
{
    BindVertexArrayObject(vao);
    BindIndexBufferObject(ibo);
}

void BindVertexArrayObject(VertexArrayObject* vao)
{
    glBindVertexArray(vao->id);    
}

void UnbindVertexArrayObject()
{
    glBindVertexArray(0);
}

