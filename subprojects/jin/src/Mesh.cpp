#include "Mesh.h"

void Mesh::Init(u32 vertices_count, u32 indices_count)
{
    vertices = std::vector<Vector3>();
    indices = std::vector<u32>();
    vertices.reserve(vertices_count);
    indices.reserve(indices_count);
    vertices_count = vertices_count;
    indices_count = indices_count;
}
