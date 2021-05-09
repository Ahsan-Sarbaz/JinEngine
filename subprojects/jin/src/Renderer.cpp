#include "Renderer.h"
#include "Application.h"
#include "Memory.h"
#include "Logger.h"
#include "Rect.h"
#include <GL/glew.h>

static Renderer* s_renderer;

Renderer* CreateRenderer(const RendererConfiguration& config)
{
    Renderer* renderer = (Renderer*)MemAlloc(sizeof(Renderer), MEMORY_TAG_STRUCT);
    renderer->config = config;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    if(config.enable_batch_renderering)
    {
        renderer->config.batch_renderer_max_vertices = 4 * config.batch_renderer_max_quads;
        renderer->config.batch_renderer_max_indices = 6 * config.batch_renderer_max_quads;
        renderer->batchData = (BatchRendererData*)MemAlloc(sizeof(BatchRendererData), MEMORY_TAG_STRUCT);
        renderer->batchData->buffer = (BatchRendererVertex*)MemAlloc(sizeof(BatchRendererVertex) * renderer->config.batch_renderer_max_vertices, MEMORY_TAG_BATCH_RENDERER_BUFFER);
        renderer->batchData->buffer_ptr = renderer->batchData->buffer;

        renderer->batchData->vao = CreateVertexArrayObject();
        renderer->batchData->vbo = CreateVertexBufferObject(VERTEX_BUFFER_OBJECT_TYPE_DYNAMIC_DRAW);
        renderer->batchData->ibo = CreateIndexBufferObject(INDEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);

        VertexBufferObjectLayout layout[5] = {
            {0, 2, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, position)},
            {1, 4, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, color)},
            {2, 2, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, uv)},
            {3, 1, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, texture_id)},
            {4, 1, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, tiling_factor)}
        };

        SetVertexBufferObjectLayout(renderer->batchData->vbo, layout, sizeof(layout) / sizeof(VertexBufferObjectLayout));
        SetVertexBufferObjectData(renderer->batchData->vbo, sizeof(BatchRendererVertex) * renderer->config.batch_renderer_max_vertices, nullptr);
        u32* indices = (u32*)MemAlloc(sizeof(u32) * renderer->config.batch_renderer_max_indices, MEMORY_TAG_BATCH_RENDERER_BUFFER);

        u32 offset = 0;
        for (u32 i = 0; i < renderer->config.batch_renderer_max_indices; i+=6)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 0;
            indices[i + 4] = offset + 2;
            indices[i + 5] = offset + 3;
            offset += 4;
        }
        
        SetIndexBufferObjectData(renderer->batchData->ibo, sizeof(u32) * renderer->config.batch_renderer_max_indices, indices);
        MemFree(indices, sizeof(u32) * renderer->config.batch_renderer_max_indices, MEMORY_TAG_BATCH_RENDERER_BUFFER);

        PushVertexBufferIntoVertexArrayObject(renderer->batchData->vao, renderer->batchData->vbo);
        PushIndexBufferIntoVertexArrayObject(renderer->batchData->vao, renderer->batchData->ibo);
        
        renderer->batchStats = (BatchRendererStats*)MemAlloc(sizeof(BatchRendererStats), MEMORY_TAG_STRUCT);
        renderer->batchStats->max_quad_count = renderer->config.batch_renderer_max_quads;
        renderer->batchStats->max_vertex_count = renderer->config.batch_renderer_max_vertices;
        renderer->batchStats->max_index_count = renderer->config.batch_renderer_max_indices;

        u8 whiteData[4] = {0xFF, 0xFF,0xFF, 0xFF};
        renderer->batchData->textures[0] = CreateTextureFromBuffer(whiteData, 1,1,4);

        char* vertex_buffer_source = 
        "#version 330 core\n"
        "layout(location = 0) in vec2 pos;\n"
        "layout(location = 1) in vec4 color;\n"
        "layout(location = 2) in vec2 uv;\n"
        "layout(location = 3) in float texture_id;\n"
        "layout(location = 4) in float tilingFactor;\n"

        "out vec4 v_color;\n"
        "out vec2 v_uv;\n"
        "out float v_tilingFactor;\n"
        "out float v_texture_id;\n"

        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 proj;\n"

        "void main()\n"
        "{\n"
            "v_color = color;\n"
            "v_uv = uv;\n"
            "v_texture_id = texture_id;\n"
            "v_tilingFactor = tilingFactor;\n"
            "gl_Position = proj * view * model * vec4(pos.x, pos.y, 0, 1);\n"
        "}\n";

        char* fragment_buffer_source = 
        "#version 330 core\n"
        "out vec4 final_color;\n"
        "in vec4 v_color;\n"
        "in vec2 v_uv;\n"
        "in float v_tilingFactor;\n"
        "in float v_texture_id;\n"
        "uniform sampler2D u_textures[16];\n"
        "void main()\n"
        "{\n"

            "vec4 color = v_color;\n"

            "switch(int(v_texture_id))\n"
            "{\n"
                "case 0: color *= texture(u_textures[0], v_uv * v_tilingFactor); break;\n"
                "case 1: color *= texture(u_textures[1], v_uv * v_tilingFactor); break;\n"
                "case 2: color *= texture(u_textures[2], v_uv * v_tilingFactor); break;\n"
                "case 3: color *= texture(u_textures[3], v_uv * v_tilingFactor); break;\n"
                "case 4: color *= texture(u_textures[4], v_uv * v_tilingFactor); break;\n"
                "case 5: color *= texture(u_textures[5], v_uv * v_tilingFactor); break;\n"
                "case 6: color *= texture(u_textures[6], v_uv * v_tilingFactor); break;\n"
                "case 7: color *= texture(u_textures[7], v_uv * v_tilingFactor); break;\n"
                "case 8: color *= texture(u_textures[8], v_uv * v_tilingFactor); break;\n"
                "case 9: color *= texture(u_textures[9], v_uv * v_tilingFactor); break;\n"
                "case 10: color *= texture(u_textures[10], v_uv * v_tilingFactor); break;\n"
                "case 11: color *= texture(u_textures[11], v_uv * v_tilingFactor); break;\n"
                "case 12: color *= texture(u_textures[12], v_uv * v_tilingFactor); break;\n"
                "case 13: color *= texture(u_textures[13], v_uv * v_tilingFactor); break;\n"
                "case 14: color *= texture(u_textures[14], v_uv * v_tilingFactor); break;\n"
                "case 15: color *= texture(u_textures[15], v_uv * v_tilingFactor); break;\n"
                "#if 0\n"
                "case 16: color *= texture(u_textures[16], v_uv * v_tilingFactor); break;\n"
                "case 17: color *= texture(u_textures[17], v_uv * v_tilingFactor); break;\n"
                "case 18: color *= texture(u_textures[18], v_uv * v_tilingFactor); break;\n"
                "case 19: color *= texture(u_textures[19], v_uv * v_tilingFactor); break;\n"
                "case 20: color *= texture(u_textures[20], v_uv * v_tilingFactor); break;\n"
                "case 21: color *= texture(u_textures[21], v_uv * v_tilingFactor); break;\n"
                "case 22: color *= texture(u_textures[22], v_uv * v_tilingFactor); break;\n"
                "case 23: color *= texture(u_textures[23], v_uv * v_tilingFactor); break;\n"
                "case 24: color *= texture(u_textures[24], v_uv * v_tilingFactor); break;\n"
                "case 25: color *= texture(u_textures[25], v_uv * v_tilingFactor); break;\n"
                "case 26: color *= texture(u_textures[26], v_uv * v_tilingFactor); break;\n"
                "case 27: color *= texture(u_textures[27], v_uv * v_tilingFactor); break;\n"
                "case 28: color *= texture(u_textures[28], v_uv * v_tilingFactor); break;\n"
                "case 29: color *= texture(u_textures[29], v_uv * v_tilingFactor); break;\n"
                "case 30: color *= texture(u_textures[30], v_uv * v_tilingFactor); break;\n"
                "case 31: color *= texture(u_textures[31], v_uv * v_tilingFactor); break;\n"
                "#endif\n"
            "}\n"

            "final_color = color;\n"
        "}\n";

        renderer->batchData->batchShader = CreateShaderProgramFromVFShaderSources(vertex_buffer_source, fragment_buffer_source);
        // renderer->batchData->batchShader = CreateShaderProgramFromVFShaderPath("res/shaders/vertex.glsl","res/shaders/fragment.glsl");
        

        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &renderer->batchData->max_textures_count);

        i32* samplers = (i32*)MemAlloc(sizeof(i32) * renderer->batchData->max_textures_count, MEMORY_TAG_STRUCT);
        for (i32 i = 0; i < renderer->batchData->max_textures_count; ++i)
        {
            samplers[i] = i;
        }
        
        UseShaderProgram(renderer->batchData->batchShader);
        SetUnifromIntArray(renderer->batchData->batchShader, "u_textures", samplers, renderer->batchData->max_textures_count);
        MemFree(samplers, sizeof(i32) * renderer->batchData->max_textures_count, MEMORY_TAG_STRUCT);   
    }

    s_renderer = renderer;
    return renderer;
}

void DeleteRenderer(Renderer* renderer)
{
    DeleteTexture(renderer->batchData->textures[0]);
    DeleteShaderProgram(renderer->batchData->batchShader);
    DeleteVertexArrayobject(renderer->batchData->vao);
    DeleteVertexBufferObject(renderer->batchData->vbo);
    DeleteIndexBufferObject(renderer->batchData->ibo);
    MemFree(renderer->batchData->buffer, sizeof(BatchRendererVertex) * renderer->config.batch_renderer_max_vertices, MEMORY_TAG_BATCH_RENDERER_BUFFER);
    MemFree(renderer->batchData, sizeof(BatchRendererData), MEMORY_TAG_STRUCT);
    MemFree(renderer->batchStats, sizeof(BatchRendererStats), MEMORY_TAG_STRUCT);
    MemFree(renderer, sizeof(Renderer), MEMORY_TAG_STRUCT);
}

void AddQuadToBuffer(const v2& position, const v2& size, const v4& color, const frect& rect, float texture_id, float tiling_factor)
{
    s_renderer->batchData->buffer_ptr->position = position;
    s_renderer->batchData->buffer_ptr->color = color;
    s_renderer->batchData->buffer_ptr->uv = {rect.x, rect.y};
    s_renderer->batchData->buffer_ptr->texture_id = texture_id;
    s_renderer->batchData->buffer_ptr->tiling_factor = tiling_factor;
    s_renderer->batchData->buffer_ptr++;

    s_renderer->batchData->buffer_ptr->position = { position.x + size.x, position.y};
    s_renderer->batchData->buffer_ptr->color = color;
    s_renderer->batchData->buffer_ptr->uv = {rect.w, rect.y};
    s_renderer->batchData->buffer_ptr->texture_id = texture_id;
    s_renderer->batchData->buffer_ptr->tiling_factor = tiling_factor;
    s_renderer->batchData->buffer_ptr++;
    
    s_renderer->batchData->buffer_ptr->position = { position.x + size.x, position.y + size.y };
    s_renderer->batchData->buffer_ptr->color = color;
    s_renderer->batchData->buffer_ptr->uv = {rect.w, rect.h};
    s_renderer->batchData->buffer_ptr->texture_id = texture_id;
    s_renderer->batchData->buffer_ptr->tiling_factor = tiling_factor;
    s_renderer->batchData->buffer_ptr++;
    
    s_renderer->batchData->buffer_ptr->position = { position.x, position.y + size.y};
    s_renderer->batchData->buffer_ptr->color = color;
    s_renderer->batchData->buffer_ptr->uv = {rect.x, rect.h};
    s_renderer->batchData->buffer_ptr->texture_id = texture_id;
    s_renderer->batchData->buffer_ptr->tiling_factor = tiling_factor;
    s_renderer->batchData->buffer_ptr++;
    s_renderer->batchData->indexCount += 6;

    s_renderer->batchStats->quad_count++;
    s_renderer->batchStats->index_count+=6;
    s_renderer->batchStats->vertex_count+=4;
}

void AddTexturedQuadToBuffer(const v2& position, const v2& size, Texture* texture, const RectF& rect, float tiling_factor, const v4& color)
{
    if(s_renderer->batchData->indexCount >= s_renderer->config.batch_renderer_max_indices)
    {
        DrawCurrentBatch(s_renderer);
        StartNewBatch(s_renderer);
    }

    if(texture == nullptr)
    {
        AddQuadToBuffer(position, size, color, rect, 0, tiling_factor);
        return;
    }

    if(s_renderer->batchData->textures_count >= s_renderer->batchData->max_textures_count)
    {
        DrawCurrentBatch(s_renderer);
        StartNewBatch(s_renderer);
    }

    int texture_unit = 0;
    for (i32 i = 1; i < s_renderer->batchData->max_textures_count; i++)
    {
        if(texture == s_renderer->batchData->textures[i])
        {
            texture_unit = i;
            break;
        }
    }

    if(texture_unit == 0)
    {        
        s_renderer->batchData->textures[s_renderer->batchData->textures_count] = texture;
        texture_unit = s_renderer->batchData->textures_count;
        s_renderer->batchData->textures_count++;

    }

    AddQuadToBuffer(position, size, color, rect, (float)texture_unit, tiling_factor);
}

void DrawQuad(const v2& position, const v2& size, const v4& color)
{
    AddTexturedQuadToBuffer(position, size, nullptr, {0,0,1,1}, 1, color);
}

void DrawTexturedQuad(const v2& position, const v2& size, Texture* texture, float tiling_factor, const v4& color)
{
    AddTexturedQuadToBuffer(position, {size.x  == 0 ? texture->width : size.x , size.y  == 0 ? texture->height : size.y} , texture, {0,0,1,1}, tiling_factor, color);
}

void DrawTexturedRectQuad(const v2& position, const v2& size, Texture* texture, const RectF& rect, float tiling_factor, const v4& color)
{
    AddTexturedQuadToBuffer(position, size, texture, rect, tiling_factor, color);
}

void StartNewBatch(Renderer* renderer)
{
    renderer->batchData->buffer_ptr = renderer->batchData->buffer;
    for (i32 i = 1; i < renderer->batchData->textures_count; i++)
    {
        renderer->batchData->textures[i] = nullptr;
    }
    
    renderer->batchData->textures_count = 1;
}

void UploadCurrentBatch(Renderer* renderer)
{
    u32 size = (u8*)renderer->batchData->buffer_ptr - (u8*)renderer->batchData->buffer;
    if(size)
    {
        SetVertexBufferObjectSubData(renderer->batchData->vbo, size, 0, renderer->batchData->buffer);
    }
}

void DrawVertexArrayObject(VertexArrayObject* vao, u32 index_count)
{
    BindVertexArrayObject(vao);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
}

void DrawCurrentBatch(Renderer* renderer)
{
    UploadCurrentBatch(renderer);
    m4 model = CreateIdentityMatrix4();
    m4 view = CreateIdentityMatrix4();
    m4 proj = CreateOrthographicProjectionMatrix4(0.0f, (float)renderer->config.app->window->config.width, (float)renderer->config.app->window->config.height, 0.0f, 0.0f, 1.0f);
    SetUniformMatrix4(renderer->batchData->batchShader, "model", model);
    SetUniformMatrix4(renderer->batchData->batchShader, "view", view);
    SetUniformMatrix4(renderer->batchData->batchShader, "proj", proj);
    UseShaderProgram(renderer->batchData->batchShader);
    
    for (u32 i = 0; i < renderer->batchData->textures_count; i++)
    {
        BindTexture(renderer->batchData->textures[i], i);
    }

    DrawVertexArrayObject(renderer->batchData->vao, renderer->batchData->indexCount);
    renderer->batchData->indexCount = 0;
    renderer->batchStats->draw_count++;
}

void ResetRendererStats(Renderer* renderer)
{
    renderer->batchStats->draw_count = 0;
    renderer->batchStats->index_count = 0;
    renderer->batchStats->vertex_count = 0;
    renderer->batchStats->quad_count = 0;
}

