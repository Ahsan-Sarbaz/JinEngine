#include "Model.h"

#include "Renderer.h"
#include "IO.h"
#include "Logger.h" 


Model::~Model()
{
    for (u32 i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
        delete vao[i];
        delete vbo_pos[i];
        delete vbo_norm[i];
        delete vbo_uv[i];
        delete ibo[i];
    }
}


void Model::ProcessMesh(aiMesh* assimp_mesh, const aiScene* scene)
{
    auto m = new Mesh(assimp_mesh->mNumVertices, assimp_mesh->mNumFaces * 3, assimp_mesh->mNumVertices, assimp_mesh->mNumVertices);
    
    for (u32 i = 0; i < assimp_mesh->mNumVertices; ++i)
    {
        m->PushVertex({assimp_mesh->mVertices[i].x, assimp_mesh->mVertices[i].y, assimp_mesh->mVertices[i].z});
        m->PushNormal({assimp_mesh->mNormals[i].x, assimp_mesh->mNormals[i].y, assimp_mesh->mNormals[i].z});
        if(assimp_mesh->HasTextureCoords(0))
        {
            m->PushUV({assimp_mesh->mTextureCoords[0][i].x, assimp_mesh->mTextureCoords[0][i].y});
        }
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
    const aiScene* scene = importer.ReadFileFromMemory(buffer, size, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_ForceGenNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        LOG_ERROR("ASSIMP: %s\n", importer.GetErrorString());
        return;
    }
    FreeFileBuffer(&buffer, size);

    ProcessNode(scene->mRootNode, scene);
    vao.reserve(meshes.size());
    vbo_pos.reserve(meshes.size());
    vbo_norm.reserve(meshes.size());
    vbo_uv.reserve(meshes.size());
    ibo.reserve(meshes.size());

    for (u32 i = 0; i < meshes.size(); i++)
    {
        vao.push_back(new VertexArrayObject());
        vbo_pos.push_back(new VertexBufferObject(VERTEX_BUFFER_OBJECT_TYPE_STATIC_DRAW));
        vbo_norm.push_back(new VertexBufferObject(VERTEX_BUFFER_OBJECT_TYPE_STATIC_DRAW));
        vbo_uv.push_back(new VertexBufferObject(VERTEX_BUFFER_OBJECT_TYPE_STATIC_DRAW));
        ibo.push_back(new IndexBufferObject(INDEX_BUFFER_OBJECT_TYPE_STATIC_DRAW));
        vao[i]->Init();

        vbo_pos[i]->SetData(sizeof(glm::vec3) * meshes[i]->GetVerticesCount(), meshes[i]->GetVerticesData());
        vbo_norm[i]->SetData(sizeof(glm::vec3) * meshes[i]->GetNormalsCount(), meshes[i]->GetNormalsData());
        vbo_uv[i]->SetData(sizeof(glm::vec2) * meshes[i]->GetUVCount(), meshes[i]->GetUVData());
        ibo[i]->SetData(sizeof(u32) * meshes[i]->GetIndicesCount(), meshes[i]->GetIndicesData());
        ibo[i]->SetCount(meshes[i]->GetIndicesCount());

        VertexBufferObjectLayout layout_pos[1] = {
            {0, 3, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, sizeof(glm::vec3), 0},
        };
        vbo_pos[i]->SetLayout(layout_pos, 1);
        
        VertexBufferObjectLayout layout_norm[1] = {
            {1, 3, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, sizeof(glm::vec3), 0},
        };
        vbo_norm[i]->SetLayout(layout_norm, 1);

        VertexBufferObjectLayout layout_uv[1] = {
            {2, 2, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, sizeof(glm::vec2), 0},
        };
        vbo_uv[i]->SetLayout(layout_uv, 1);


        vao[i]->PushVertexBuffer(vbo_pos[i]);
        vao[i]->PushVertexBuffer(vbo_norm[i]);
        vao[i]->PushVertexBuffer(vbo_uv[i]);
        vao[i]->PushIndexBuffer(ibo[i]);
    }
}

Model::Model(Mesh* mesh)
{
    AddMesh(mesh);

    vao.reserve(meshes.size());
    vbo_pos.reserve(meshes.size());
    vbo_norm.reserve(meshes.size());
    vbo_uv.reserve(meshes.size());
    ibo.reserve(meshes.size());

    for (u32 i = 0; i < meshes.size(); i++)
    {
        vao[i] = new VertexArrayObject();
        vbo_pos[i] = new VertexBufferObject(VERTEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);
        vbo_norm[i] = new VertexBufferObject(VERTEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);
        vbo_uv[i] = new VertexBufferObject(VERTEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);
        ibo[i] = new IndexBufferObject(INDEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);    
        vao[i]->Init();

        vbo_pos[i]->SetData(sizeof(glm::vec3) * meshes[i]->GetVerticesCount(), meshes[i]->GetVerticesData());
        vbo_norm[i]->SetData(sizeof(glm::vec3) * meshes[i]->GetNormalsCount(), meshes[i]->GetNormalsData());
        vbo_uv[i]->SetData(sizeof(glm::vec2) * meshes[i]->GetUVCount(), meshes[i]->GetUVData());
        ibo[i]->SetData(sizeof(u32) * meshes[i]->GetIndicesCount(), meshes[i]->GetIndicesData());
        ibo[i]->SetCount(meshes[i]->GetIndicesCount());

        VertexBufferObjectLayout layout_pos[1] = {
            {0, 3, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, sizeof(glm::vec3), 0},
        };
        vbo_pos[i]->SetLayout(layout_pos, 1);
        
        VertexBufferObjectLayout layout_norm[1] = {
            {1, 3, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, sizeof(glm::vec3), 0},
        };
        vbo_norm[i]->SetLayout(layout_norm, 1);

        VertexBufferObjectLayout layout_uv[1] = {
            {2, 2, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, sizeof(glm::vec2), 0},
        };
        vbo_uv[i]->SetLayout(layout_uv, 1);


        vao[i]->PushVertexBuffer(vbo_pos[i]);
        vao[i]->PushVertexBuffer(vbo_norm[i]);
        vao[i]->PushVertexBuffer(vbo_uv[i]);
        vao[i]->PushIndexBuffer(ibo[i]);
    }
}


void Model::AddMesh(Mesh* mesh)
{
    meshes.push_back(mesh);
}