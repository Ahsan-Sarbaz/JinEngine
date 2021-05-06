#include "MainLayer.h"
#include "Jin.h"

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


Texture* texture;
SpriteSheet* ss;

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

    texture = CreateTexture("res/textures/running.png");
    SpriteSheetConfiguration ssConfig = {};
    ssConfig.texture = texture;
    ssConfig.sprite_width = 108;
    ssConfig.sprite_height = 140;
    ssConfig.padding = 0;
    ss = CreateSpriteSheet(ssConfig);

    LOG_INFO("Sprites Per Col %d\n", ss->sprites_per_column);
}

void MainLayerUpdate(Application* app)
{   
    StartNewBatch(app->renderer);

    // DrawTexturedRectQuad({0,0}, {}, texture, {0.0f,0.0f,0.125f, 0.5f});
    DrawTexturedRectQuad({0,0}, {ss->config.sprite_width, ss->config.sprite_height}, texture, ss->rects[1]);
    // for(i32 i = 0; i < 8; ++i)
    // {
    //     for(i32 j = 0; j < 8; ++j)
    //     {
    //         if(map[i][j] == WALL)
    //             DrawQuad({(float)j * 32.0f, (float)i * 32.0f}, {32.0f, 32.0f}, {1,0,0,1});

    //             //DrawQuad({(float)j * 32.0f, (float)i * 32.0f}, {32.0f, 32.0f}, {1,0,0,1});
            
    //         if(playerPos.x == j && playerPos.y == i)
    //         {
    //             DrawQuad({(float)j * 32.0f, (float)i * 32.0f}, {32.0f, 32.0f}, {1,1,0,1});
    //             // DrawQuad({(float)j * 32.0f, (float)i * 32.0f}, {32.0f, 32.0f}, {1,1,0,1});
    //         }
    //     }
    // }
   
    DrawCurrentBatch(app->renderer);
    
}

void MainLayerEnd(Application* app)
{
    DeleteTexture(texture);
    DeleteSpriteSheet(ss);
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