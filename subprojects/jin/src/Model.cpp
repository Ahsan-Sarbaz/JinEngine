#include "Model.h"

#include "IO.h"
#include "Logger.h"


Model::~Model()
{
    for (u32 i = 0; i < mesh_count; i++)
    {
        delete meshes[i];
        delete vao[i];
        delete vbo[i];
        delete ibo[i];
    }
}


void Model::ProcessMesh(aiMesh* assimp_mesh, const aiScene* scene)
{
    auto m = new Mesh(assimp_mesh->mNumVertices, assimp_mesh->mNumFaces * 3);
    
    for (u32 i = 0; i < assimp_mesh->mNumVertices; ++i)
    {
        m->PushVertex({assimp_mesh->mVertices[i].x, assimp_mesh->mVertices[i].y, assimp_mesh->mVertices[i].z});
    }

    for (u32 i = 0; i < assimp_mesh->mNumFaces; ++i)
    {
        auto face = assimp_mesh->mFaces[i];
        for (u32 j = 0; j < face.mNumIndices; ++j)
        {
            m->PushIndex(face.mIndices[j]);
        }        
    }
    AddMesh(m);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (u32 i = 0; i < node->mNumMeshes; ++i)
    {
        ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene);    
    }

    for (u32 i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Model::Model(const char* path)
{
    Assimp::Importer importer;
    char* buffer;
    i32 size;
    ReadFileToBuffer(path, &buffer, &size);
    const aiScene* scene = importer.ReadFileFromMemory(buffer, size, aiProcess_Triangulate | aiProcess_FlipUVs);
    FreeFileBuffer(&buffer, size);

    ProcessNode(scene->mRootNode, scene);
    vao.reserve(mesh_count);
    vbo.reserve(mesh_count);
    ibo.reserve(mesh_count);

    for (u32 i = 0; i < mesh_count; i++)
    {
        vao[i] = new VertexArrayObject();
        vbo[i] = new VertexBufferObject(VERTEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);
        ibo[i] = new IndexBufferObject(INDEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);    
        vao[i]->Init();

        vbo[i]->SetData(sizeof(vec3) * meshes[i]->GetVerticesCount(), meshes[i]->GetVerticesData());
        ibo[i]->SetData(sizeof(u32) * meshes[i]->GetIndicesCount(), meshes[i]->GetIndicesData());
        ibo[i]->SetCount(meshes[i]->GetIndicesCount());

        VertexBufferObjectLayout layout[1] = {
            {0, 3, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, sizeof(vec3), 0}
        };

        vbo[i]->SetLayout(layout, 1);

        vao[i]->PushVertexBuffer(vbo[i]);
        vao[i]->PushIndexBuffer(ibo[i]);
    }
}


void Model::AddMesh(Mesh* mesh)
{
    meshes.push_back(mesh);
    mesh_count++;
}
