#include "Mesh.h"

Mesh::Mesh(u32 vertices_count, u32 normals_count, u32 uv_count, u32 indices_count)
{
    vertices = std::vector<glm::vec3>();
    normals = std::vector<glm::vec3>();
    uvs = std::vector<glm::vec2>();
    indices = std::vector<u32>();
    vertices.reserve(vertices_count);
    uvs.reserve(uv_count);
    normals.reserve(normals_count);
    indices.reserve(indices_count);
}


Mesh* Mesh::CreateSphereMesh(u32 sengments)
{
    Mesh* m = new Mesh();

    float radius = 1.0f;
    float x, y, z, xy;                              
    float nx, ny, nz, lengthInv = 1.0f / radius;    
    float s, t;                                     

    float sectorStep = 2 * M_PI / (float)sengments;
    float stackStep = M_PI / (float)sengments;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= sengments; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sengments; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            m->PushVertex({x, y, z});
            

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            m->PushNormal({nx, ny, nz});

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / (float)sengments;
            t = (float)i / (float)sengments;
            m->PushUV({s, t});
        }
    }

    int k1, k2;
    for(int i = 0; i < sengments; ++i)
    {
        k1 = i * (sengments + 1);     // beginning of current stack
        k2 = k1 + sengments + 1;      // beginning of next stack

        for(int j = 0; j < sengments; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                m->PushIndex(k1);
                m->PushIndex(k2);
                m->PushIndex(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (sengments-1))
            {
                m->PushIndex(k1 + 1);
                m->PushIndex(k2);
                m->PushIndex(k2 + 1);
            }
        }
    }

    return m;
}

Mesh* Mesh::CreateCubeMesh()
{
    Mesh* m = new Mesh();

    m->PushVertex({-1.0f, 1.0f, -1.0f});
    m->PushVertex({ 1.0f, 1.0f, -1.0f});
    m->PushVertex({ 1.0f, -1.0f, -1.0f});
    m->PushVertex({ -1.0f, -1.0f, -1.0f});
    m->PushVertex({-1.0f, 1.0f, 1.0f});
    m->PushVertex({ 1.0f, 1.0f, 1.0f});
    m->PushVertex({ 1.0f, -1.0f, 1.0f});
    m->PushVertex({ -1.0f, -1.0f, 1.0f});

    m->PushNormal({1.0f, 1.0f, -1.0f});
    m->PushNormal({ 1.0f, 1.0f, -1.0f});
    m->PushNormal({ 1.0f, -1.0f, -1.0f});
    m->PushNormal({ -1.0f, -1.0f, -1.0f});
    m->PushNormal({-1.0f, 1.0f, 1.0f});
    m->PushNormal({ 1.0f, 1.0f, 1.0f});
    m->PushNormal({ 1.0f, -1.0f, 1.0f});
    m->PushNormal({ -1.0f, -1.0f, 1.0f});


    m->PushIndex(0);
    m->PushIndex(1);
    m->PushIndex(2);
    m->PushIndex(0);
    m->PushIndex(2);
    m->PushIndex(3);

    m->PushIndex(5);
    m->PushIndex(4);
    m->PushIndex(7);
    m->PushIndex(5);
    m->PushIndex(7);
    m->PushIndex(6);

    m->PushIndex(1);
    m->PushIndex(0);
    m->PushIndex(4);
    m->PushIndex(1);
    m->PushIndex(4);
    m->PushIndex(5);

    m->PushIndex(3);
    m->PushIndex(2);
    m->PushIndex(6);
    m->PushIndex(3);
    m->PushIndex(6);
    m->PushIndex(7);

    m->PushIndex(0);
    m->PushIndex(3);
    m->PushIndex(7);
    m->PushIndex(0);
    m->PushIndex(7);
    m->PushIndex(4);

    m->PushIndex(1);
    m->PushIndex(5);
    m->PushIndex(6);
    m->PushIndex(1);
    m->PushIndex(6);
    m->PushIndex(2);

    return m;
}

