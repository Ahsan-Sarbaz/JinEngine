#include "Mesh.h"

Mesh::Mesh(u32 vertices_count, u32 indices_count)
{
    vertices = std::vector<glm::vec3>();
    indices = std::vector<u32>();
    vertices.reserve(vertices_count);
    indices.reserve(indices_count);
    vertices_count = vertices_count;
    indices_count = indices_count;
}
