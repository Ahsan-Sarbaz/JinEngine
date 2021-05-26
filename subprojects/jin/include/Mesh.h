#pragma once
#include "Types.h"
#include "Maths.h"
#include <vector>

class Mesh
{
    std::vector<Vector3> vertices;
    std::vector<u32> indices;

public:
    Mesh() = default;
    void Init(u32 vertices_count, u32 indices_count);
    inline void PushVertex(const Vector3& v) { vertices.push_back(v); }
    inline void PushIndex(u32 i) { indices.push_back(i); }

    inline u32 GetVerticesCount() { return vertices.size(); }
    inline u32 GetIndicesCount() { return indices.size(); }

    inline Vector3* GetVerticesData() { return vertices.data(); }
    inline u32* GetIndicesData() { return indices.data(); }
};