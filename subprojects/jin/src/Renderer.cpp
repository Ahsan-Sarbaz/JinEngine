#include "Renderer.h"
#include "Memory.h"
#include "Logger.h"
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

        VertexBufferObjectLayout layout[2] = {
            {0, 2, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, position)},
            {1, 4, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, color)}
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
    }


    s_renderer = renderer;
    return renderer;
}

void DeleteRenderer(Renderer* renderer)
{
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

void DrawQuadBatched(const v2& position, const v2& size, const v4& color)
{
    StartNewBatch();
    if(s_renderer->batchData->indexCount >= s_renderer->config.batch_renderer_max_indices)
    {
        DrawCurrentBatch();
        StartNewBatch();
    }

    s_renderer->batchData->buffer_ptr->position = position;
    s_renderer->batchData->buffer_ptr->color = color;
    s_renderer->batchData->buffer_ptr++;
    s_renderer->batchData->buffer_ptr->position = { position.x + size.x, position.y};
    s_renderer->batchData->buffer_ptr->color = color;
    s_renderer->batchData->buffer_ptr++;
    s_renderer->batchData->buffer_ptr->position = { position.x + size.x, position.y + size.y };
    s_renderer->batchData->buffer_ptr->color = color;
    s_renderer->batchData->buffer_ptr++;
    s_renderer->batchData->buffer_ptr->position = { position.x, position.y + size.y};
    s_renderer->batchData->buffer_ptr->color = color;
    s_renderer->batchData->buffer_ptr++;
    s_renderer->batchData->indexCount += 6;

    s_renderer->batchStats->quad_count++;
    s_renderer->batchStats->index_count+=6;
    s_renderer->batchStats->vertex_count+=4;
}

void StartNewBatch()
{
    if(!s_renderer->batchData->is_batch_started)
    {
        s_renderer->batchData->buffer_ptr = s_renderer->batchData->buffer;
        s_renderer->batchData->is_batch_started = TRUE;
    }
}

void UploadCurrentBatch()
{
    if(!s_renderer->batchData->is_batch_uploaded)
    {
        u32 size = (u8*)s_renderer->batchData->buffer_ptr - (u8*)s_renderer->batchData->buffer;
        if(size)
        {
            SetVertexBufferObjectSubData(s_renderer->batchData->vbo, size, 0, s_renderer->batchData->buffer);
        }
        s_renderer->batchData->is_batch_uploaded = TRUE;
    }
}

void DrawVertexArrayObject(VertexArrayObject* vao, u32 index_count)
{
    BindVertexArrayObject(vao);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
}

void DrawCurrentBatch()
{
    if(s_renderer->batchData->indexCount)
    {
        UploadCurrentBatch();

        DrawVertexArrayObject(s_renderer->batchData->vao, s_renderer->batchData->indexCount);
        s_renderer->batchData->indexCount = 0;

        s_renderer->batchStats->draw_count++;

        s_renderer->batchData->is_batch_uploaded = FALSE;
        s_renderer->batchData->is_batch_started = FALSE;
    }
}

void ResetRendererStats()
{
    s_renderer->batchStats->draw_count = 0;
    s_renderer->batchStats->index_count = 0;
    s_renderer->batchStats->vertex_count = 0;
    s_renderer->batchStats->quad_count = 0;
}

