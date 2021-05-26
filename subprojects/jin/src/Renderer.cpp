#include "Renderer.h"
#include "Application.h"
#include "Logger.h"
#include "Rect.h"
#include "Time.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>


void Renderer::Init(const RendererConfiguration& _config)
{
    this->config = _config;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_DEPTH_TEST);

    if(config.enable_batch_renderering)
    {
        config.batch_renderer_max_vertices = 4 * config.batch_renderer_max_quads;
        config.batch_renderer_max_indices = 6 * config.batch_renderer_max_quads;
        batchData = new BatchRendererData();
        batchData->buffer = (BatchRendererVertex*)malloc(sizeof(BatchRendererVertex) * config.batch_renderer_max_vertices);
        batchData->buffer_ptr = batchData->buffer;

        batchData->vao = new VertexArrayObject();
        batchData->vbo = new VertexBufferObject();
        batchData->ibo = new IndexBufferObject();

        batchData->vao->Init();
        batchData->vbo->Init(VERTEX_BUFFER_OBJECT_TYPE_DYNAMIC_DRAW);
        batchData->ibo->Init(INDEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);

        VertexBufferObjectLayout layout[5] = {
            {0, 2, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, position)},
            {1, 4, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, color)},
            {2, 2, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, uv)},
            {3, 1, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, texture_id)},
            {4, 1, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, tiling_factor)}
        };

        batchData->vbo->SetLayout(layout, sizeof(layout) / sizeof(VertexBufferObjectLayout));
        batchData->vbo->SetData(sizeof(BatchRendererVertex) * config.batch_renderer_max_vertices, nullptr);
        u32* indices = (u32*)malloc(sizeof(u32) * config.batch_renderer_max_indices);

        u32 offset = 0;
        for (u32 i = 0; i < config.batch_renderer_max_indices; i+=6)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 0;
            indices[i + 4] = offset + 2;
            indices[i + 5] = offset + 3;
            offset += 4;
        }
        
        batchData->ibo->SetData(sizeof(u32) * config.batch_renderer_max_indices, indices);
        free(indices);

        batchData->vao->PushVertexBuffer(batchData->vbo);
        batchData->vao->PushIndexBuffer(batchData->ibo);
        
        batchStats = (BatchRendererStats*)malloc(sizeof(BatchRendererStats));
        batchStats->max_quad_count = config.batch_renderer_max_quads;
        batchStats->max_vertex_count = config.batch_renderer_max_vertices;
        batchStats->max_index_count = config.batch_renderer_max_indices;

        u8 whiteData[4] = {0xFF, 0xFF,0xFF, 0xFF};
        batchData->textures[0] = new Texture();
        batchData->textures[0]->InitFromBuffer(whiteData, 1,1,4);

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

        batchData->batchShader = new ShaderProgram();
        batchData->batchShader->InitFromVFShaderSources(vertex_buffer_source, fragment_buffer_source);

        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &batchData->max_textures_count);

        i32* samplers = (i32*)malloc(sizeof(i32) * batchData->max_textures_count);
        for (i32 i = 0; i < batchData->max_textures_count; ++i)
        {
            samplers[i] = i;
        }
        
        batchData->batchShader->Use();
        batchData->batchShader->SetUnifromIntArray("u_textures", samplers, batchData->max_textures_count);
        free(samplers);   
    }

    rendererData = new ThreeDRendererData();
    char* v_source = "#version 330 core\n"
    "layout(location=0) in vec3 pos;\n"
    "uniform mat4 u_proj;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_model;\n"
    "out vec3 v_color;\n"
    "void main() {\n"
    "v_color = pos;"
    "gl_Position = u_proj * u_view * u_model * vec4(pos, 1);"
    "}\n";
    char* f_source = "#version 330 core\n"
    "out vec4 final_color;\n"
    "in vec3 v_color;\n"
    "void main() {\n"
    "final_color = vec4(v_color,1);"
    "}\n";

    rendererData->shader = new ShaderProgram();
    rendererData->shader->InitFromVFShaderSources(v_source, f_source);
}

// void DeleteRenderer(Renderer* renderer)
// {
//     DeleteTexture(renderer->batchData->textures[0]);
//     DeleteShaderProgram(renderer->batchData->batchShader);
//     DeleteVertexArrayobject(renderer->batchData->vao);
//     DeleteVertexBufferObject(renderer->batchData->vbo);
//     DeleteIndexBufferObject(renderer->batchData->ibo);
//     MemFree(renderer->batchData->buffer, sizeof(BatchRendererVertex) * renderer->config.batch_renderer_max_vertices, MEMORY_TAG_BATCH_RENDERER_BUFFER);
//     MemFree(renderer->batchData, sizeof(BatchRendererData), MEMORY_TAG_STRUCT);
//     MemFree(renderer->batchStats, sizeof(BatchRendererStats), MEMORY_TAG_STRUCT);

//     MemFree(renderer->rendereData, sizeof(ThreeDRendererData), MEMORY_TAG_STRUCT);
//     DeleteShaderProgram(renderer->rendereData->shader);


//     MemFree(renderer, sizeof(Renderer), MEMORY_TAG_STRUCT);
// }

void Renderer::AddQuadToBuffer(const v2& position, const v2& size, const v4& color, const frect& rect, float texture_id, float tiling_factor)
{
    batchData->buffer_ptr->position = position;
    batchData->buffer_ptr->color = color;
    batchData->buffer_ptr->uv = {rect.x, rect.y};
    batchData->buffer_ptr->texture_id = texture_id;
    batchData->buffer_ptr->tiling_factor = tiling_factor;
    batchData->buffer_ptr++;

    batchData->buffer_ptr->position = { position.x + size.x, position.y};
    batchData->buffer_ptr->color = color;
    batchData->buffer_ptr->uv = {rect.w, rect.y};
    batchData->buffer_ptr->texture_id = texture_id;
    batchData->buffer_ptr->tiling_factor = tiling_factor;
    batchData->buffer_ptr++;
    
    batchData->buffer_ptr->position = { position.x + size.x, position.y + size.y };
    batchData->buffer_ptr->color = color;
    batchData->buffer_ptr->uv = {rect.w, rect.h};
    batchData->buffer_ptr->texture_id = texture_id;
    batchData->buffer_ptr->tiling_factor = tiling_factor;
    batchData->buffer_ptr++;
    
    batchData->buffer_ptr->position = { position.x, position.y + size.y};
    batchData->buffer_ptr->color = color;
    batchData->buffer_ptr->uv = {rect.x, rect.h};
    batchData->buffer_ptr->texture_id = texture_id;
    batchData->buffer_ptr->tiling_factor = tiling_factor;
    batchData->buffer_ptr++;
    batchData->indexCount += 6;

    batchStats->quad_count++;
    batchStats->index_count+=6;
    batchStats->vertex_count+=4;
}

void Renderer::AddTexturedQuadToBuffer(const v2& position, const v2& size, Texture* texture, const RectF& rect, float tiling_factor, const v4& color)
{
    if(batchData->indexCount >= config.batch_renderer_max_indices)
    {
        DrawCurrentBatch();
        StartNewBatch();
    }

    if(texture == nullptr)
    {
        AddQuadToBuffer(position, size, color, rect, 0, tiling_factor);
        return;
    }

    if(batchData->textures_count >= batchData->max_textures_count)
    {
        DrawCurrentBatch();
        StartNewBatch();
    }

    int texture_unit = 0;
    for (i32 i = 1; i < batchData->max_textures_count; i++)
    {
        if(texture == batchData->textures[i])
        {
            texture_unit = i;
            break;
        }
    }

    if(texture_unit == 0)
    {        
        batchData->textures[batchData->textures_count] = texture;
        texture_unit = batchData->textures_count;
        batchData->textures_count++;

    }

    AddQuadToBuffer(position, size, color, rect, (float)texture_unit, tiling_factor);
}

void Renderer::DrawQuad(const v2& position, const v2& size, const v4& color)
{
    AddTexturedQuadToBuffer(position, size, nullptr, {0,0,1,1}, 1, color);
}

void Renderer::DrawTexturedQuad(const v2& position, const v2& size, Texture* texture, float tiling_factor, const v4& color)
{
    AddTexturedQuadToBuffer(position, {size.x  == 0 ? texture->GetWidth() : size.x , size.y  == 0 ? texture->GetHeight() : size.y} , texture, {0,0,1,1}, tiling_factor, color);
}

void Renderer::DrawTexturedRectQuad(const v2& position, const v2& size, Texture* texture, const RectF& rect, float tiling_factor, const v4& color)
{
    AddTexturedQuadToBuffer(position, size, texture, rect, tiling_factor, color);
}

void Renderer::StartNewBatch()
{
    batchData->buffer_ptr = batchData->buffer;
    for (i32 i = 1; i < batchData->textures_count; i++)
    {
        batchData->textures[i] = nullptr;
    }
    
    batchData->textures_count = 1;
}

void Renderer::UploadCurrentBatch()
{
    u32 size = (u8*)batchData->buffer_ptr - (u8*)batchData->buffer;
    if(size)
    {
        batchData->vbo->SetSubData(size, 0, batchData->buffer);
    }
}

void Renderer::DrawVertexArrayObject(VertexArrayObject* vao, u32 index_count)
{
    vao->Bind();
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawCurrentBatch()
{
    UploadCurrentBatch();
    m4 model = CreateIdentityMatrix4();
    m4 view = CreateIdentityMatrix4();
    m4 proj = CreateOrthographicProjectionMatrix4(0.0f, (float)config.app->GetWindow()->GetConfig()->width, (float)config.app->GetWindow()->GetConfig()->height, 0.0f, 0.0f, 1.0f);
    batchData->batchShader->SetUniformMatrix4("model", model);
    batchData->batchShader->SetUniformMatrix4("view", view);
    batchData->batchShader->SetUniformMatrix4("proj", proj);
    batchData->batchShader->Use();
    
    for (u32 i = 0; i < batchData->textures_count; i++)
    {
        batchData->textures[i]->Bind(i);
    }

    DrawVertexArrayObject(batchData->vao, batchData->indexCount);
    batchData->indexCount = 0;
    batchStats->draw_count++;
}

void Renderer::ResetRendererStats()
{
    batchStats->draw_count = 0;
    batchStats->index_count = 0;
    batchStats->vertex_count = 0;
    batchStats->quad_count = 0;
}

float angle = 0;

void Renderer::DrawModel(Model* model)
{
    for (u32 i = 0; i < model->GetMeshCount(); i++)
    {
        angle += 25.0f * GetDeltaTimeMs();
        auto modelmat = glm::rotate(glm::identity<glm::mat4>(), glm::radians(angle), {0.0f, 1.0f,0.0f});
        auto viewmat = glm::lookAt(glm::vec3{0.0f,0.0f,10.0f}, glm::vec3{0.0f,0.0f,0.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
        // m4 modelmat = CreateIdentityMatrix4();
        m4 projmat = CreatePerspectiveProjectionMatrix4(75.0f, (float)config.app->GetWindow()->GetConfig()->width,
         (float)config.app->GetWindow()->GetConfig()->height, 0.1f, 1000.0f);

        rendererData->shader->SetUniformMatrix4("u_model", modelmat);
        rendererData->shader->SetUniformMatrix4("u_view", viewmat);
        rendererData->shader->SetUniformMatrix4("u_proj", projmat);
        rendererData->shader->Use();
        
        DrawVertexArrayObject(model->GetVAO(i), model->GetIBO(i)->GetCount());
    }   
}