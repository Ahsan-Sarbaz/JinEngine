#include "Mesh.h"
#include "Memory.h"

Mesh* CreateMesh(u32 vertices_count, u32 indices_count)
{
    Mesh* m = (Mesh*)MemAlloc(sizeof(Mesh), MEMORY_TAG_STRUCT);
    m->vertices = (vec3*)MemAlloc(sizeof(vec3) * vertices_count, MEMORY_TAG_STRUCT);
    m->indices = (u32*)MemAlloc(sizeof(u32) * indices_count, MEMORY_TAG_STRUCT);
    m->vertices_count = vertices_count;
    m->indices_count = indices_count;

    return m;
}

void DeleteMesh(Mesh* mesh)
{
    MemFree(mesh->vertices, sizeof(vec3) * mesh->vertices_count, MEMORY_TAG_STRUCT);
    MemFree(mesh->indices, sizeof(u32) * mesh->indices_count, MEMORY_TAG_STRUCT);
    MemFree(mesh, sizeof(Mesh), MEMORY_TAG_STRUCT);
}