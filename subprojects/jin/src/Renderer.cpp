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

        renderer->batchData->batchShader = CreateShaderProgramFromVFShaderPath("res/shaders/vertex.glsl","res/shaders/fragment.glsl");
        

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

void SetOrthoProjection(const v2& size)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double)size.x, (double)size.y, 0.0, -1.0, 1.0);
}

void DrawQuad(const v2& position, const v2& size, const v4& color)
{
    glBegin(GL_QUADS);
    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(position.x, position.y);

    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(position.x, position.y + size.y);
    
    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(position.x + size.x, position.y + size.y);
    
    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(position.x + size.x, position.y);
    glEnd();
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

void DrawQuadBatched(const v2& position, const v2& size, const v4& color)
{
    if(s_renderer->batchData->indexCount >= s_renderer->config.batch_renderer_max_indices)
    {
        DrawCurrentBatch();
        StartNewBatch();
    }

    AddQuadToBuffer(position, size, color, {0,0,1,1}, 0, 1);
}


void DrawTexturedQuadBatched(const v2& position, const v2& size, Texture* texture, float tiling_factor, const v4& color)
{
    if(s_renderer->batchData->textures_count >= s_renderer->batchData->max_textures_count)
    {
        DrawCurrentBatch();
        StartNewBatch();
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

    AddQuadToBuffer(position, size, color, {0,0,1,1}, (float)texture_unit, tiling_factor);    
}

void StartNewBatch()
{
    s_renderer->batchData->buffer_ptr = s_renderer->batchData->buffer;
    for (i32 i = 1; i < s_renderer->batchData->textures_count; i++)
    {
        s_renderer->batchData->textures[i] = nullptr;
    }
    
    s_renderer->batchData->textures_count = 1;
    
}

void UploadCurrentBatch()
{
    u32 size = (u8*)s_renderer->batchData->buffer_ptr - (u8*)s_renderer->batchData->buffer;
    if(size)
    {
        SetVertexBufferObjectSubData(s_renderer->batchData->vbo, size, 0, s_renderer->batchData->buffer);
    }
}

void DrawVertexArrayObject(VertexArrayObject* vao, u32 index_count)
{
    BindVertexArrayObject(vao);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
}

void DrawCurrentBatch()
{
    UploadCurrentBatch();
    m4 model = CreateIdentityMatrix4();
    m4 view = CreateIdentityMatrix4();
    m4 proj = CreateOrthographicProjectionMatrix4(0.0f, (float)s_renderer->config.app->window->config.width, (float)s_renderer->config.app->window->config.height, 0.0f, 0.0f, 1.0f);
    SetUniformMatrix4(s_renderer->batchData->batchShader, "model", model);
    SetUniformMatrix4(s_renderer->batchData->batchShader, "view", view);
    SetUniformMatrix4(s_renderer->batchData->batchShader, "proj", proj);
    UseShaderProgram(s_renderer->batchData->batchShader);
    
    for (u32 i = 0; i < s_renderer->batchData->textures_count; i++)
    {
        BindTexture(s_renderer->batchData->textures[i], i);
    }

    DrawVertexArrayObject(s_renderer->batchData->vao, s_renderer->batchData->indexCount);
    s_renderer->batchData->indexCount = 0;
    // LOG_TRACE("Draw Call\n");
    s_renderer->batchStats->draw_count++;
}

void ResetRendererStats()
{
    s_renderer->batchStats->draw_count = 0;
    s_renderer->batchStats->index_count = 0;
    s_renderer->batchStats->vertex_count = 0;
    s_renderer->batchStats->quad_count = 0;
}

