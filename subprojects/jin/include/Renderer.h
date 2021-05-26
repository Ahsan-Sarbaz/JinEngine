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
#include "Model.h"

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

struct ThreeDRendererData
{
    ShaderProgram* shader;
};

class Renderer
{
    RendererConfiguration config;
    BatchRendererData* batchData;
    BatchRendererStats* batchStats;
    ThreeDRendererData* rendererData;

    public:
    Renderer() = default;

    void Init(const RendererConfiguration& config);
    void DrawQuad(const v2& position, const v2& size, const v4& color);
    void DrawTexturedQuad(const v2& position, const v2& size, Texture* texture, float tiling_factor = 1.0f, const v4& color = {1.0f,1.0f,1.0f,1.0f});
    void DrawTexturedRectQuad(const v2& position, const v2& size, Texture* texture, const RectF& rect, float tiling_factor = 1.0f, const v4& color = {1.0f,1.0f,1.0f,1.0f});
    void StartNewBatch();
    void UploadCurrentBatch();
    void DrawCurrentBatch();
    void DrawVertexArrayObject(VertexArrayObject* vao, u32 index_count);
    void ResetRendererStats();
    
    
    // 3D renderer

    void DrawModel(Model* model);

    inline RendererConfiguration* GetConfig() { return &config; }
    inline BatchRendererData* GetBatchData() { return batchData; }
    inline BatchRendererStats* GetBatchStats() { return batchStats; }
    inline ThreeDRendererData* Get3DData() { return rendererData; }

    private:
    
    void AddQuadToBuffer(const v2& position, const v2& size, const v4& color, const frect& rect, float texture_id, float tiling_factor);
    void AddTexturedQuadToBuffer(const v2& position, const v2& size, Texture* texture, const RectF& rect, float tiling_factor, const v4& color);

  
};

