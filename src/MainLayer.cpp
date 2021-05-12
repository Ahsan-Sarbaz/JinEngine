#include "MainLayer.h"
#include "Jin.h"
#include "Entity.h"
#include "Components.h"


Texture* test;
SpriteSheet* ss;
SpriteSheetAnimation* animation;
void MainLayerStart(Application* app)
{
    test = CreateTexture("res/textures/running.png");
    SpriteSheetConfiguration ssConfig = {};
    ssConfig.texture = test;
    ssConfig.sprite_width = 108;
    ssConfig.sprite_height = 140;
    ss = CreateSpriteSheet(ssConfig);
    SpriteSheetAnimationLayout layout[2] = {};
    layout[0].name = "running";
    layout[0].start_frame = 0;
    layout[0].end_frame = 8;
    layout[0].total_time = 0.7f;
    
    layout[1].name = "running-back";
    layout[1].start_frame = 9;
    layout[1].end_frame = 16;
    layout[1].total_time = 0.7f;
    
    animation = CreateSpriteSheetAnimation(ss, layout[0]);
    CreateSpriteSheetAnimationEntity("boy", animation, {{}, {}, {100, 100, 0}});
}

void MainLayerUpdate(Application* app)
{   

}

void MainLayerEnd(Application* app)
{
    DeleteSpriteSheet(ss);
    DeleteSpriteSheetAnimation(animation);
    DeleteTexture(test);
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