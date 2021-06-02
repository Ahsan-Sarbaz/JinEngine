#pragma once
#include "Types.h"
#include <vector>

class Mesh
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<u32> indices;

public:
    Mesh() = default;
    Mesh(u32 vertices_count, u32 normals_count, u32 uv_count, u32 indices_count);

    static Mesh* CreateSphereMesh(u32 sengments = 32);
    static Mesh* CreateCubeMesh();

    inline void PushVertex(const glm::vec3& v) { vertices.push_back(v); }
    inline void PushNormal(const glm::vec3& n) { normals.push_back(n); }
    inline void PushUV(const glm::vec2& uv) { uvs.push_back(uv); }
    inline void PushIndex(u32 i) { indices.push_back(i); }

    inline u32 GetVerticesCount() { return vertices.size(); }
    inline u32 GetNormalsCount() { return normals.size(); }
    inline u32 GetUVCount() { return uvs.size(); }
    inline u32 GetIndicesCount() { return indices.size(); }

    inline glm::vec3* GetVerticesData() { return vertices.data(); }
    inline glm::vec3* GetNormalsData() { return normals.data(); }
    inline glm::vec2* GetUVData() { return uvs.data(); }
    inline u32* GetIndicesData() { return indices.data(); }
};