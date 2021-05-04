#include "MainLayer.h"
#include "imgui.h"
#include "Jin.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum MAP_LEGEND
{
    WALL = 1,
    PLAYER_LOC = 2,
};

const u32 map[8][8] = {
    {1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1},
    {1,0,2,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,1},
    {1,0,0,0,1,0,0,1},
    {1,1,1,1,1,1,1,1},
};

vec2 playerPos = {1,1};

ShaderProgram* program;

void MainLayerStart(Application* app)
{
    for(i32 i = 0; i < 8; ++i)
    {
        for(i32 j = 0; j < 8; ++j)
        {
            if(map[i][j] == PLAYER_LOC)
            {
                playerPos = {(float)j , (float)i};
            }
        }
    }

    program = CreateShaderProgramFromVFShaderPath("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
}

void MainLayerUpdate(Application* app)
{   
    for(i32 i = 0; i < 8; ++i)
    {
        for(i32 j = 0; j < 8; ++j)
        {
            if(map[i][j] == WALL)
                DrawQuadBatched({(float)j * 32.0f, (float)i * 32.0f}, {32.0f, 32.0f}, {1,0,0,1});
                //DrawQuad({(float)j * 32.0f, (float)i * 32.0f}, {32.0f, 32.0f}, {1,0,0,1});
            
            if(playerPos.x == j && playerPos.y == i)
            {
                DrawQuadBatched({(float)j * 32.0f, (float)i * 32.0f}, {32.0f, 32.0f}, {1,1,0,1});
                // DrawQuad({(float)j * 32.0f, (float)i * 32.0f}, {32.0f, 32.0f}, {1,1,0,1});
            }
        }
    }

    glm::mat4 model = glm::identity<glm::mat4>();
    glm::mat4 view = glm::identity<glm::mat4>();
    glm::mat4 proj = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 0.0f, 1.0f);
    SetUniformMatrix4(program, "model", (Matrix4)model);
    SetUniformMatrix4(program, "view", (Matrix4)view);
    SetUniformMatrix4(program, "proj", (Matrix4)proj);

    UseShaderProgram(program);
    DrawCurrentBatch();
    
}

void MainLayerEnd(Application* app)
{
    DeleteShaderProgram(program);
}

Layer* GetMainLayer(Application* app)
{
    LayerConfiguration config;
    config.name = "Main Layer";
    config.onStart = MainLayerStart;
    config.onUpdate = MainLayerUpdate;
    config.onEnd = MainLayerEnd;

    return CreateLayer(config);
}