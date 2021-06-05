#pragma once
#include "Types.h"
#include "Rect.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "CubeMap.h"
#include "Material.h"
#include "UniformBufferObject.h"

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
    glm::vec2 position;
    glm::vec4 color;
    glm::vec2 uv;
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
    VAO* cubeVao;
    VBO* cubeVbo;
    IBO* cubeIbo;

    Material* defaultMat;
    Material* skyboxMat;



    UniformBufferObject* cameraUBO;
};

class Renderer
{
public:
    struct CameraUBOLayout
    {
        glm::mat4 projection;
        glm::mat4 view;
        glm::vec3 position;
    };

private:
    static Renderer* s_renderer;
    RendererConfiguration config;
    BatchRendererData* batchData;
    BatchRendererStats* batchStats;
    ThreeDRendererData* rendererData;

public:
    Renderer() = default;
    ~Renderer();
    Renderer(const RendererConfiguration& config);
    inline static Renderer* GetInstance() { return s_renderer; }
    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    void DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size, Texture* texture, float tiling_factor = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
    void DrawTexturedRectQuad(const glm::vec2& position, const glm::vec2& size, Texture* texture, const RectF& rect, float tiling_factor = 1.0f, const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f});
    void StartNewBatch();
    void UploadCurrentBatch();
    void DrawCurrentBatch();
    void DrawVertexArrayObject(VertexArrayObject* vao, u32 index_count);
    void ResetRendererStats();
    
    // 3D renderer
    void DrawModel(Model* model, Material* material = nullptr);
    void DrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, Material* material = nullptr);
    void DrawSkybox(CubeMap* map);

    inline RendererConfiguration* GetConfig() { return &config; }
    inline BatchRendererData* GetBatchData() { return batchData; }
    inline BatchRendererStats* GetBatchStats() { return batchStats; }
    inline ThreeDRendererData* GetRendererData() { return rendererData; }
    static inline Material* GetDefaultMaterial() { return s_renderer->GetRendererData()->defaultMat; }
    static void SetCameraUBOData(CameraUBOLayout data);
    private:
    
    void AddQuadToBuffer(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const frect& rect, float texture_id, float tiling_factor);
    void AddTexturedQuadToBuffer(const glm::vec2& position, const glm::vec2& size, Texture* texture, const RectF& rect, float tiling_factor, const glm::vec4& color);
};

