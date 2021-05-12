#pragma once
#include "Types.h"
#include "SpriteSheet.h"

struct SpriteSheetAnimationLayout
{
    const char* name;
    i32 start_frame;
    i32 end_frame;
    float frames_per_second;
    float total_time;

    i32 current_frame;
    float time_step;
    float time_accumulator;
};


struct SpriteSheetAnimation
{
    SpriteSheet* sprite_sheet;
    SpriteSheetAnimationLayout layout;
};

SpriteSheetAnimation* CreateSpriteSheetAnimation(SpriteSheet* sprite_sheet, const SpriteSheetAnimationLayout& layout);
void DeleteSpriteSheetAnimation(SpriteSheetAnimation* ss_animation);
void UpdateSpriteSheetAnimation(SpriteSheetAnimation* ss_animation);