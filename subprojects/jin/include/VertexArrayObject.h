#pragma once
#include "Types.h"

struct VertexBufferObject;
struct IndexBufferObject;


struct VertexArrayObject
{
    u32 id;
};


typedef VertexArrayObject VAO;

VertexArrayObject* CreateVertexArrayObject();
void DeleteVertexArrayobject(VertexArrayObject* vao);
void PushVertexBufferIntoVertexArrayObject(VertexArrayObject* vao, VertexBufferObject* vbo);
void PushIndexBufferIntoVertexArrayObject(VertexArrayObject* vao, IndexBufferObject* ibo);
void BindVertexArrayObject(VertexArrayObject* vao);
void UnbindVertexArrayObject();
