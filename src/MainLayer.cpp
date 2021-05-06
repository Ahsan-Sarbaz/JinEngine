#include "MainLayer.h"
#include "Jin.h"

enum MAP_LEGEND
{
    WALL = 1,
    PLAYER_LOC = 2,
};

b8 Chance(i32 percent)
{
    return (b8)((rand() % 100) + 1 > (percent));
}

constexpr i32 mapWidth = 32;
constexpr i32 mapHeight = 32;

u32 map[mapWidth][mapHeight] = {
    {01,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {01,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,01},
    {00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00},
};

vec2 playerPos = {1,1};


Texture* texture;
Texture* explosionTexture;
SpriteSheet* ss;
SpriteSheet* explosionSpriteSheet;
SpriteSheetAnimation* animation;
SpriteSheetAnimation* explosionAnimation;

void MainLayerStart(Application* app)
{
    for(i32 i = 0; i < mapWidth; ++i)
    {
        for(i32 j = 0; j < mapHeight; ++j)
        {
            i32 index = map[i][j];
            if(index == 65)
            {
                map[i][j] = Chance(70) ? index : 64;
                // map[i][j] = (rand() % 100 - 20) % 2 ? index : 12;
            }

            index = map[i][j];
            if(index == 64)
            {
                map[i][j] = Chance(30) ? index : 12;
            }

            index = map[i][j];
            if(index == 12)
            {
                map[i][j] = Chance(30) ? index : 30;
            }

            index = map[i][j];
            if(index == 12)
            {
                map[i][j] = Chance(30) ? index : 38;
            }
        }
    }

    // texture = CreateTexture("res/textures/tileset.png");
    texture = CreateTexture("res/textures/running.png");
    explosionTexture = CreateTexture("res/textures/explosion-4.png");

    SpriteSheetConfiguration ssConfig = {};
    ssConfig.texture = texture;
    ssConfig.sprite_width = 108;
    ssConfig.sprite_height = 140;
    ssConfig.padding = 0;
    // ss = CreateSpriteSheet(ssConfig);
    ss = CreateSpriteSheet(ssConfig);
    SpriteSheetAnimationLayout layout[2] ={};

    layout[0].name = "Running";
    layout[0].start_frame = 0;
    layout[0].end_frame = 8;
    layout[0].total_time = 0.5f;

    layout[1].name = "Running Backward";
    layout[1].start_frame = 9;
    layout[1].end_frame = 16;
    layout[1].total_time = 0.5f;

    animation = CreateSpriteSheetAnimation(ss, layout, 2);

    SpriteSheetConfiguration explosionConfig = {};
    explosionConfig.sprite_width = 128.0f;
    explosionConfig.sprite_height = 128.0f;
    explosionConfig.texture = explosionTexture;
    explosionSpriteSheet = CreateSpriteSheet(explosionConfig);

    SpriteSheetAnimationLayout expLayout[1] = {};
    expLayout[0].name = "Explosion";
    expLayout[0].start_frame = 0;
    expLayout[0].end_frame  = explosionSpriteSheet->sprites_count;
    expLayout[0].total_time = 1.0f;

    explosionAnimation = CreateSpriteSheetAnimation(explosionSpriteSheet, expLayout, 1);

}

void MainLayerUpdate(Application* app)
{   
    StartNewBatch(app->renderer);

    // for(i32 i = 0; i < mapWidth; ++i)
    // {
    //     for(i32 j = 0; j < mapHeight; ++j)
    //     {

    //         i32 index = map[i][j];
    //         if(index == 30 || index == 38)
    //         {
    //             DrawTexturedRectQuad({(float)j * 16.0f, (float)i * 16.0f}, {ss->config.sprite_width, ss->config.sprite_height}, texture, ss->rects[11]);
    //         }

    //         LOG_TRACE("%f DT\n", GetDeltaTime());

    //         DrawTexturedRectQuad({(float)j * 16.0f, (float)i * 16.0f}, {ss->config.sprite_width, ss->config.sprite_height}, texture, ss->rects[index]);
    //         // if(playerPos.x == j && playerPos.y == i)
    //         // {
    //         //     DrawQuad({(float)j * 32.0f, (float)i * 32.0f}, {32.0f, 32.0f}, {1,1,0,1});
    //         //     // DrawQuad({(float)j * 32.0f, (float)i * 32.0f}, {32.0f, 32.0f}, {1,1,0,1});
    //         // }
    //     }
    // }


    // DrawTexturedRectQuad({0,0}, {104 * 2, 140 * 2},texture, ss->rects[animation->layouts[0]->current_frame]);
    // DrawTexturedRectQuad({0,140 * 2}, {104 * 2, 140 * 2},texture, ss->rects[animation->layouts[1]->current_frame]);
    DrawTexturedRectQuad({0,0}, {128, 128}, explosionTexture, explosionSpriteSheet->rects[explosionAnimation->layouts[0]->current_frame]);
    // DrawTexturedRectQuad({0,0}, {104 * 2, 140 * 2},texture, ss->rects[2]);

    UpdateSpriteSheetAnimation(animation);
    UpdateSpriteSheetAnimation(explosionAnimation);
    
   
    DrawCurrentBatch(app->renderer);
}

void MainLayerEnd(Application* app)
{
    DeleteSpriteSheetAnimation(animation);
    DeleteSpriteSheetAnimation(explosionAnimation);
    DeleteTexture(texture);
    DeleteTexture(explosionTexture);
    DeleteSpriteSheet(ss);
    DeleteSpriteSheet(explosionSpriteSheet);
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