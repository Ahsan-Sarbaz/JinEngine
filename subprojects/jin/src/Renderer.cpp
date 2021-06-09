#include "Renderer.h"
#include "Application.h"
#include "Logger.h"
#include "Rect.h"
#include "Time.h"

#include <GL/glew.h>

Renderer* Renderer::s_renderer;

Renderer::Renderer(const RendererConfiguration& _config)
{
    s_renderer = this;
    config = _config;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);  

    if(config.enable_batch_renderering)
    {
        config.batch_renderer_max_vertices = 4 * config.batch_renderer_max_quads;
        config.batch_renderer_max_indices = 6 * config.batch_renderer_max_quads;
        batchData = new BatchRendererData();
        batchData->buffer = new BatchRendererVertex[config.batch_renderer_max_vertices];
        batchData->buffer_ptr = batchData->buffer;

        batchData->vao = new VertexArrayObject();
        batchData->vbo = new VertexBufferObject(VERTEX_BUFFER_OBJECT_TYPE_DYNAMIC_DRAW);
        batchData->ibo = new IndexBufferObject(INDEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);

        batchData->vao->Init();

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
        
        batchStats = new BatchRendererStats();
        batchStats->max_quad_count = config.batch_renderer_max_quads;
        batchStats->max_vertex_count = config.batch_renderer_max_vertices;
        batchStats->max_index_count = config.batch_renderer_max_indices;

        u8 whiteData[4] = {0xFF, 0xFF,0xFF, 0xFF};
        batchData->textures[0] = new Texture();
        batchData->textures[0]->InitFromBuffer(whiteData, 1,1,4);

        char* vertex_buffer_source = R"(
        #version 330 core
        
        layout(location = 0) in vec2 pos;
        layout(location = 1) in vec4 color;
        layout(location = 2) in vec2 uv;
        layout(location = 3) in float texture_id;
        layout(location = 4) in float tilingFactor;

        out vec4 v_color;
        out vec2 v_uv;
        out float v_tilingFactor;
        out float v_texture_id;

        uniform mat4 u_model;
        uniform mat4 u_view;
        uniform mat4 u_proj;

        void main()
        {
            v_color = color;
            v_uv = uv;
            v_texture_id = texture_id;
            v_tilingFactor = tilingFactor;
            gl_Position = u_proj * u_view * u_model * vec4(pos.x, pos.y, 0, 1);
        }
        )";

        char* fragment_buffer_source = R"(
        #version 330 core

        out vec4 final_color;
        
        in vec4 v_color;
        in vec2 v_uv;
        in float v_tilingFactor;
        in float v_texture_id;
        
        uniform sampler2D u_textures[16];
        
        void main()
        {
            vec4 color = v_color;
            switch(int(v_texture_id))
            {
                case 0: color *= texture(u_textures[0], v_uv * v_tilingFactor); break;
                case 1: color *= texture(u_textures[1], v_uv * v_tilingFactor); break;
                case 2: color *= texture(u_textures[2], v_uv * v_tilingFactor); break;
                case 3: color *= texture(u_textures[3], v_uv * v_tilingFactor); break;
                case 4: color *= texture(u_textures[4], v_uv * v_tilingFactor); break;
                case 5: color *= texture(u_textures[5], v_uv * v_tilingFactor); break;
                case 6: color *= texture(u_textures[6], v_uv * v_tilingFactor); break;
                case 7: color *= texture(u_textures[7], v_uv * v_tilingFactor); break;
                case 8: color *= texture(u_textures[8], v_uv * v_tilingFactor); break;
                case 9: color *= texture(u_textures[9], v_uv * v_tilingFactor); break;
                case 10: color *= texture(u_textures[10], v_uv * v_tilingFactor); break;
                case 11: color *= texture(u_textures[11], v_uv * v_tilingFactor); break;
                case 12: color *= texture(u_textures[12], v_uv * v_tilingFactor); break;
                case 13: color *= texture(u_textures[13], v_uv * v_tilingFactor); break;
                case 14: color *= texture(u_textures[14], v_uv * v_tilingFactor); break;
                case 15: color *= texture(u_textures[15], v_uv * v_tilingFactor); break;
                #if 0
                case 16: color *= texture(u_textures[16], v_uv * v_tilingFactor); break;
                case 17: color *= texture(u_textures[17], v_uv * v_tilingFactor); break;
                case 18: color *= texture(u_textures[18], v_uv * v_tilingFactor); break;
                case 19: color *= texture(u_textures[19], v_uv * v_tilingFactor); break;
                case 20: color *= texture(u_textures[20], v_uv * v_tilingFactor); break;
                case 21: color *= texture(u_textures[21], v_uv * v_tilingFactor); break;
                case 22: color *= texture(u_textures[22], v_uv * v_tilingFactor); break;
                case 23: color *= texture(u_textures[23], v_uv * v_tilingFactor); break;
                case 24: color *= texture(u_textures[24], v_uv * v_tilingFactor); break;
                case 25: color *= texture(u_textures[25], v_uv * v_tilingFactor); break;
                case 26: color *= texture(u_textures[26], v_uv * v_tilingFactor); break;
                case 27: color *= texture(u_textures[27], v_uv * v_tilingFactor); break;
                case 28: color *= texture(u_textures[28], v_uv * v_tilingFactor); break;
                case 29: color *= texture(u_textures[29], v_uv * v_tilingFactor); break;
                case 30: color *= texture(u_textures[30], v_uv * v_tilingFactor); break;
                case 31: color *= texture(u_textures[31], v_uv * v_tilingFactor); break;
                #endif
            }
            final_color = color;
        }
        )";

        batchData->batchShader = new ShaderProgram();
        batchData->batchShader->InitFromVFShaderSources(vertex_buffer_source, fragment_buffer_source);

        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &batchData->max_textures_count);

        i32* samplers = (i32*)malloc(sizeof(i32) * batchData->max_textures_count);
        for (i32 i = 0; i < batchData->max_textures_count; ++i)
        {
            samplers[i] = i;
        }
        
        batchData->batchShader->Bind();
        batchData->batchShader->SetUnifromIntArray("u_textures", samplers, batchData->max_textures_count);
        batchData->batchShader->Unbind();
        free(samplers);   
    }

    rendererData = new ThreeDRendererData();
    char* v_source = R"(
    #version 330 core
    layout(location=0) in vec3 pos;
    layout(location=1) in vec3 norm;

    layout (std140) uniform Camera
    {
       mat4 projection;
       mat4 view;
    } cam;
    uniform mat4 u_model;
    out vec3 v_color;
    void main() 
    {
        v_color = norm;
        gl_Position = cam.projection * cam.view * u_model * vec4(pos, 1);
    })";
    char* f_source = R"(
    #version 330 core
    out vec4 final_color;
    in vec3 v_color;
    void main() 
    {
       final_color = vec4(v_color, 1);
    })";

    rendererData->defaultMat = new Material(new ShaderProgram());
    rendererData->defaultMat->GetProgram()->InitFromVFShaderSources(v_source, f_source);

    char* skybox_v_source = R"(
    #version 330 core
    layout(location=0) in vec3 a_Pos;
    
    layout (std140) uniform Camera
    {
       mat4 projection;
       mat4 view;
       vec3 position;
    } cam;

    out vec3 v_textureDir;

    void main() 
    {
        vec4 pos = cam.projection * cam.view * vec4(vec3(a_Pos.x * 100000000000.0, a_Pos.y * 100000000000.0, a_Pos.z * 100000000000.0), 1);
        gl_Position= pos.xyww;
        v_textureDir = a_Pos;
    })";
    char* skybox_f_source = R"(
    #version 330 core
    out vec4 final_color;

    in vec3 v_textureDir;
    uniform samplerCube skyBox;

    void main()
    {
       final_color = texture(skyBox,v_textureDir);
    })";


    rendererData->skyboxMat = new Material(new ShaderProgram());
    rendererData->skyboxMat->GetProgram()->InitFromVFShaderSources(skybox_v_source, skybox_f_source);


    u32 cameraUniformBlockIndex = glGetUniformBlockIndex(rendererData->defaultMat->GetProgram()->GetId(), "Camera");
    glUniformBlockBinding(rendererData->defaultMat->GetProgram()->GetId(), cameraUniformBlockIndex, 0);


    float cubeVerts[] = {
        // top (+z)
        -1, -1,  1,
         1, -1,  1,
        -1,  1,  1,
        -1,  1,  1,
         1, -1,  1,
         1,  1,  1, 

        // bottom (-z)
        -1, -1, -1,
        -1,  1, -1,
         1, -1, -1,
         1, -1, -1,
        -1,  1, -1,
         1,  1, -1,

        // right (+x)
         1, -1, -1, 
         1,  1, -1, 
         1, -1,  1, 
         1, -1,  1, 
         1,  1, -1, 
         1,  1,  1, 

        // left (-x)
        -1, -1, -1, 
        -1, -1,  1, 
        -1,  1, -1, 
        -1,  1, -1, 
        -1, -1,  1, 
        -1,  1,  1, 

        // front (+y)
        -1, -1, -1,
         1, -1, -1,
        -1, -1,  1,
        -1, -1,  1,
         1, -1, -1,
         1, -1,  1,

        // back (-y)
        -1,  1, -1,
        -1,  1,  1,
         1,  1, -1,
         1,  1, -1,
        -1,  1,  1,
         1,  1,  1,
    };

    rendererData->cubeVao = new VertexArrayObject();
    rendererData->cubeVao->Init();    
    rendererData->cubeVbo = new VertexBufferObject(VERTEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);
    VertexBufferObjectLayout cubeVBOLayout[1] = {
        {0, 3, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(float) * 3, (void*)0},
        // {1, 4, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, color)},
        // {2, 2, VERTEX_BUFFER_OBJECT_LAYOUT_TYPE_FLOAT, 0, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, uv)},
    };
    rendererData->cubeVbo->SetLayout(cubeVBOLayout, sizeof(cubeVBOLayout) / sizeof(VertexBufferObjectLayout));
    rendererData->cubeVbo->SetData(sizeof(cubeVerts), cubeVerts);
    rendererData->cubeVao->PushVertexBuffer(rendererData->cubeVbo);
    rendererData->cubeIbo = new IBO(INDEX_BUFFER_OBJECT_TYPE_STATIC_DRAW);
    u32 cubeIndices[] = {
        0, 1, 2, 
        3, 4, 5,

        6, 7, 8,
        9,10,11,

        12,13,14,
        15,16,17,

        18,19,20,
        21,22,23,

        24,25,26,
        27,28,29,

        30,31,32,
        33,34,35
    };
    rendererData->cubeIbo->SetData(sizeof(cubeIndices), cubeIndices);
    rendererData->cubeIbo->SetCount(sizeof(cubeIndices) / sizeof(u32));
    rendererData->cubeVao->PushIndexBuffer(rendererData->cubeIbo);

    rendererData->cameraUBO = new UniformBufferObject(UNIFORM_BUFFER_OBJECT_TYPE_DYNAMIC_DRAW);
    rendererData->cameraUBO->SetBindingIndex(0);
    rendererData->cameraUBO->SetData(sizeof(CameraUBOLayout), nullptr);
}   

Renderer::~Renderer()
{

    delete[] batchData->buffer;
    delete batchData->batchShader;
    delete batchData->vao;
    delete batchData->vbo;
    delete batchData->ibo;
    delete batchData->textures[0];
    delete batchData;

    delete rendererData->defaultMat->GetProgram();
    delete rendererData->defaultMat;
    delete rendererData->skyboxMat->GetProgram();
    delete rendererData->skyboxMat;

    delete rendererData->cubeVao;
    delete rendererData->cubeVbo;
    delete rendererData->cubeIbo;

    delete rendererData;

    delete batchStats;
}


void Renderer::AddQuadToBuffer(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const frect& rect, float texture_id, float tiling_factor)
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

void Renderer::AddTexturedQuadToBuffer(const glm::vec2& position, const glm::vec2& size, Texture* texture, const RectF& rect, float tiling_factor, const glm::vec4& color)
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

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    AddTexturedQuadToBuffer(position, size, nullptr, {0.0f, 0.0f,1.0f,1.0f}, 1, color);
}

void Renderer::DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size, Texture* texture, float tiling_factor, const glm::vec4& color)
{
    AddTexturedQuadToBuffer(position, {size.x  == 0 ? texture->GetWidth() : size.x , size.y  == 0 ? texture->GetHeight() : size.y} , texture, {0.0f,0.0f,1.0f,1.0f}, tiling_factor, color);
}

void Renderer::DrawTexturedRectQuad(const glm::vec2& position, const glm::vec2& size, Texture* texture, const RectF& rect, float tiling_factor, const glm::vec4& color)
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
    vao->Unbind();
}

void Renderer::DrawCurrentBatch()
{
    if(batchData->indexCount > 0)
    {

        UploadCurrentBatch();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::ortho(0.0f, (float)config.app->GetWindow()->GetConfig()->width, (float)config.app->GetWindow()->GetConfig()->height, 0.0f, 0.0f, 1.0f);

        batchData->batchShader->Bind();
        batchData->batchShader->SetUniformMatrix4("u_model", model);
        batchData->batchShader->SetUniformMatrix4("u_view", view);
        batchData->batchShader->SetUniformMatrix4("u_proj", proj);
        
        for (u32 i = 0; i < batchData->textures_count; i++)
        {
            batchData->textures[i]->Bind(i);
        }

        DrawVertexArrayObject(batchData->vao, batchData->indexCount);
        batchData->batchShader->Unbind();
        batchData->indexCount = 0;
        batchStats->draw_count++;
    }
}

void Renderer::ResetRendererStats()
{
    batchStats->draw_count = 0;
    batchStats->index_count = 0;
    batchStats->vertex_count = 0;
    batchStats->quad_count = 0;
}

void Renderer::DrawModel(Model* model, const glm::mat4& model_matrix, Material* material)
{
    if(material == nullptr) material = rendererData->defaultMat;
    material->GetProgram()->Bind();
    material->GetProgram()->SetUniformMatrix4("u_model", model_matrix);
    for (u32 i = 0; i < model->GetMeshCount(); i++)
    {
        DrawVertexArrayObject(model->GetVAO(i), model->GetIBO(i)->GetCount());
    }
    material->GetProgram()->Unbind();
}

void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, Material* material)
{
    if(material == nullptr) material = rendererData->defaultMat;
    material->GetProgram()->Bind();
    DrawVertexArrayObject(rendererData->cubeVao, rendererData->cubeIbo->GetCount());
    material->GetProgram()->Unbind();
}

void Renderer::DrawSkybox(CubeMap* map)
{
    glCullFace(GL_FRONT);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    rendererData->skyboxMat->GetProgram()->Bind();
    map->Bind();
    DrawCube({}, {}, {}, rendererData->skyboxMat);
    map->Unbind();
    rendererData->skyboxMat->GetProgram()->Unbind();
    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void Renderer::SetCameraUBOData(CameraUBOLayout data)
{
    s_renderer->rendererData->cameraUBO->SetSubData(0, sizeof(CameraUBOLayout), &data);
}
