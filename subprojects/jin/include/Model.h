#pragma once
#include "Types.h"
#include "Mesh.h"
#include <vector>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"

struct Model
{
    std::vector<Mesh*> meshes;
    u32 mesh_count;

    std::vector<VAO*> vao;
    std::vector<VBO*> vbo;
    std::vector<IBO*> ibo;
};

Model* CreateModel();
Model* CreateModelFromFile(const char* path);
void DeleteModel(Model* model);
void AddMeshToModel(Model* model, Mesh* mesh);