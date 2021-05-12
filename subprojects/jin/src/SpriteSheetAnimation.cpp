#include "SpriteSheetAnimation.h"
#include "Logger.h"
#include "Memory.h"
#include "Time.h"

SpriteSheetAnimation* CreateSpriteSheetAnimation(SpriteSheet* sprite_sheet, const SpriteSheetAnimationLayout& layout)
{
    SpriteSheetAnimation* anim = (SpriteSheetAnimation*)MemAlloc(sizeof(SpriteSheetAnimation), MEMORY_TAG_STRUCT);
    anim->sprite_sheet = sprite_sheet;

    anim->layout.name = layout.name;
    anim->layout.start_frame = layout.start_frame;
    anim->layout.end_frame = layout.end_frame;
    anim->layout.total_time = layout.total_time;
    
    anim->layout.current_frame = layout.start_frame;
    anim->layout.frames_per_second = ((float)layout.end_frame - (float)layout.start_frame) / layout.total_time;
    anim->layout.time_step = 1.0f / anim->layout.frames_per_second;

    return anim;
}

void DeleteSpriteSheetAnimation(SpriteSheetAnimation* ss_animation)
{
    MemFree(ss_animation, sizeof(SpriteSheetAnimation) , MEMORY_TAG_STRUCT);
}

void UpdateSpriteSheetAnimation(SpriteSheetAnimation* ss_animation)
{
    ss_animation->layout;
    auto dt = GetDeltaTimeMs();
    ss_animation->layout.time_accumulator += (float) dt;
    if(ss_animation->layout.time_accumulator >= ss_animation->layout.time_step)
    {
        ss_animation->layout.current_frame++;
        if(ss_animation->layout.current_frame >= ss_animation->layout.end_frame)
            ss_animation->layout.current_frame = ss_animation->layout.start_frame;
        ss_animation->layout.time_accumulator = 0;
    }
}