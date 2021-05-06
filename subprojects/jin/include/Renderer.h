#pragma once
#include "Types.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Rect.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include "Texture.h"
#include "ShaderProgram.h"

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
    v2 uv;
    float texture_id;
    float tiling_factor;
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
    Texture* textures[16];
    u32 textures_count;
    i32 max_textures_count;
    ShaderProgram* batchShader;
};

struct Renderer
{
    RendererConfiguration config;
    BatchRendererData* batchData;
    BatchRendererStats* batchStats;
};

Renderer* CreateRenderer(const RendererConfiguration& config);
void DeleteRenderer(Renderer* renderer);

void DrawQuad(const v2& position, const v2& size, const v4& color);
void DrawTexturedQuad(const v2& position, const v2& size, Texture* texture, float tiling_factor = 1.0f, const v4& color = {1.0f,1.0f,1.0f,1.0f});
void DrawTexturedRectQuad(const v2& position, const v2& size, Texture* texture, const RectF& rect, float tiling_factor = 1.0f, const v4& color = {1.0f,1.0f,1.0f,1.0f});
void StartNewBatch(Renderer* renderer);
void UploadCurrentBatch(Renderer* renderer);
void DrawCurrentBatch(Renderer* renderer);
void DrawVertexArrayObject(VertexArrayObject* vao, u32 index_count);
void ResetRendererStats(Renderer* renderer);