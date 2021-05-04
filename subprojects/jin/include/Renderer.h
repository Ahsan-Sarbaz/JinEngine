#pragma once
#include "Types.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"

struct Application;

struct RendererConfiguration
{
    Application* app;
    u32 renderer_resolution_x;
    u32 renderer_resolution_y; 
    b8 enable_batch_renderering;
    u32 batch_renderer_max_quads;
    u32 batch_renderer_max_vertices;
    u32 batch_renderer_max_indices;
};

struct BatchRendererVertex
{
    v2 position;
    v4 color;
};

struct BatchRendererStats
{
    u32 vertex_count;
    u32 index_count;
    u32 quad_count;
    u32 max_vertex_count;
    u32 max_index_count;
    u32 max_quad_count;
    u32 draw_count;
};

struct BatchRendererData
{
    BatchRendererVertex* buffer;
    BatchRendererVertex* buffer_ptr;
    VAO* vao;
    VBO* vbo;
    IBO* ibo;
    u32 indexCount;
    b8 is_batch_started;
    b8 is_batch_uploaded;
};

struct Renderer
{
    RendererConfiguration config;
    BatchRendererData* batchData;
    BatchRendererStats* batchStats;
};

Renderer* CreateRenderer(const RendererConfiguration& config);
void DeleteRenderer(Renderer* renderer);
/// this function uses opengl immidiate calls so don't use it for something serious
void SetOrthoProjection(const v2& size);
/// this function uses opengl immidiate calls so don't use it for something serious
void DrawQuad(const v2& position, const v2& size, const v4& color);

void DrawQuadBatched(const v2& position, const v2& size, const v4& color);
void StartNewBatch();
void UploadCurrentBatch();
void DrawCurrentBatch();
void DrawVertexArrayObject(VertexArrayObject* vao, u32 index_count);
void ResetRendererStats();