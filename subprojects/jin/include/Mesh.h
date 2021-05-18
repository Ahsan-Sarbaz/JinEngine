#pragma once
#include "Types.h"
#include "Maths.h"

struct Mesh
{
    Vector3* vertices;
    u32 vertices_count;

    u32* indices;
    u32 indices_count;
};

Mesh* CreateMesh(u32 vertices_count, u32 indices_count);
void DeleteMesh(Mesh* mesh);