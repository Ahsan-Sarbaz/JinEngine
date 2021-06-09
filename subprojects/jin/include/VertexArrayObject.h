#pragma once
#include "Types.h"

class VertexBufferObject;
class IndexBufferObject;


class VertexArrayObject
{
private:
    u32 id;
public:
    VertexArrayObject() = default;
    void Init();
    void PushVertexBuffer(VertexBufferObject* vbo);
    void PushIndexBuffer(IndexBufferObject* ibo);
    void Bind();
    void Unbind();

};

typedef VertexArrayObject VAO;
