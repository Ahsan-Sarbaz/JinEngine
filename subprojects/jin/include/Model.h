#pragma once
#include "Types.h"
#include "Mesh.h"
#include <vector>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Model
{
private:
    std::vector<Mesh*> meshes;
    std::vector<VAO*> vao;
    std::vector<VBO*> vbo_pos;
    std::vector<VBO*> vbo_norm;
    std::vector<VBO*> vbo_uv;
    std::vector<IBO*> ibo;

public:
    Model() = default;
    ~Model();
    Model(const char* path);
    Model(Mesh* mesh);
    void AddMesh(Mesh* mesh);
    inline u32 GetMeshCount() { return meshes.size(); }
    inline VAO* GetVAO(u32 index) { return vao[index]; }
    inline VBO* GetVBOPos(u32 index) { return vbo_pos[index]; }
    inline VBO* GetVBONorm(u32 index) { return vbo_norm[index]; }
    inline VBO* GetVBOUV(u32 index) { return vbo_uv[index]; }
    inline IBO* GetIBO(u32 index) { return ibo[index]; }

private:
    void ProcessMesh(aiMesh* assimp_mesh, const aiScene* scene);
    void ProcessNode(aiNode* node, const aiScene* scene);
};
