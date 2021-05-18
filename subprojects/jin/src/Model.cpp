#include "Model.h"
#include "Memory.h"
#include "IO.h"
#include "Logger.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Model* CreateModel()
{
    Model* model = (Model*)MemAlloc(sizeof(Model), MEMORY_TAG_STRUCT);
    return model;
}

void ProcessMesh(Model* model, aiMesh* assimp_mesh, const aiScene* scene)
{
    auto m = CreateMesh(assimp_mesh->mNumVertices, assimp_mesh->mNumFaces * 3);
    
    for (u32 i = 0; i < assimp_mesh->mNumVertices; ++i)
    {
        m->vertices[i] = {assimp_mesh->mVertices[i].x, assimp_mesh->mVertices[i].y, assimp_mesh->mVertices[i].z};
    }

    u32 ind = 0;
    for (u32 i = 0; i < assimp_mesh->mNumFaces; ++i)
    {
        auto face = assimp_mesh->mFaces[i];
        for (u32 j = 0; j < face.mNumIndices; ++j)
        {
            m->indices[ind] = face.mIndices[j];
            ind++;
        }        
    }

    AddMeshToModel(model, m);
}

void ProcessNode(Model* model, aiNode* node, const aiScene* scene)
{
    for (u32 i = 0; i < node->mNumMeshes; ++i)
    {
        ProcessMesh(model, scene->mMeshes[node->mMeshes[i]], scene);    
    }

    for (u32 i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(model, node->mChildren[i], scene);
    }
}

Model* CreateModelFromFile(const char* path)
{
    Model* model = CreateModel();
    
    Assimp::Importer importer;
    char* buffer;
    i32 size;
    ReadFileToBuffer(path, &buffer, &size);
    const aiScene* scene = importer.ReadFileFromMemory(buffer, size, aiProcess_Triangulate | aiProcess_FlipUVs);
    FreeFileBuffer(&buffer, size);

    ProcessNode(model, scene->mRootNode, scene);

    // model->vao = (VAO**)MemAlloc(sizeof(VAO*) * model->mesh_count, MEMORY_TAG_STRUCT);
    // model->vbo = (VBO**)MemAlloc(sizeof(VBO*) * model->mesh_count, MEMORY_TAG_STRUCT);
    // model->ibo = (IBO**)MemAlloc(sizeof(IBO*) * model->mesh_count, MEMORY_TAG_STRUCT);

    model->vao.reserve(model->mesh_count);
    model->vbo.reserve(model->mesh_count);
    model->ibo.reserve(model->mesh_count);

    for (u32 i = 0; i < model->mesh_count; i++)
    {
        model->vao[i] = CreateVertexArrayObject();
        model->vbo[i] = CreateVertexBufferObject(VERTEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);
        model->ibo[i] = CreateIndexBufferObject(INDEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);

        SetVertexBufferObjectData(model->vbo[i], sizeof(vec3) * model->meshes[i]->vertices_count, model->meshes[i]->vertices);
        SetIndexBufferObjectData(model->ibo[i], sizeof(u32) * model->meshes[i]->indices_count, model->meshes[i]->indices);
        SetIndexBufferIndicesCount(model->ibo[i], model->meshes[i]->indices_count);

        VertexBufferObjectLayout layout[1] = {
            {0, 3, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, sizeof(vec3), 0}
        };

        SetVertexBufferObjectLayout(model->vbo[i], layout, 1);
        SetIndexBufferIndicesCount(model->ibo[i], model->meshes[i]->indices_count);

        PushVertexBufferIntoVertexArrayObject(model->vao[i], model->vbo[i]);
        PushIndexBufferIntoVertexArrayObject(model->vao[i], model->ibo[i]);
    }
    
    return model;
}

void DeleteModel(Model* model)
{
    for (u32 i = 0; i < model->mesh_count; i++)
    {
        DeleteMesh(model->meshes[i]);
        DeleteVertexArrayobject(model->vao[i]);
        DeleteVertexBufferObject(model->vbo[i]);
        DeleteIndexBufferObject(model->ibo[i]);
    }

    // MemFree(model->vao, sizeof(VAO*) * model->mesh_count, MEMORY_TAG_STRUCT);
    // MemFree(model->vbo, sizeof(VBO*) * model->mesh_count, MEMORY_TAG_STRUCT);
    // MemFree(model->ibo, sizeof(IBO*) * model->mesh_count, MEMORY_TAG_STRUCT);
    
    MemFree(model, sizeof(Model), MEMORY_TAG_STRUCT);
}

void AddMeshToModel(Model* model, Mesh* mesh)
{
    model->meshes.push_back(mesh);
    model->mesh_count++;
}
