#include "SpriteSheetAnimation.h"
#include "Logger.h"
#include "Memory.h"
#include "Time.h"

SpriteSheetAnimation* CreateSpriteSheetAnimation(SpriteSheet* sprite_sheet, SpriteSheetAnimationLayout* layouts, i32 count)
{
    SpriteSheetAnimation* anim = (SpriteSheetAnimation*)MemAlloc(sizeof(SpriteSheetAnimation), MEMORY_TAG_STRUCT);
    anim->sprite_sheet = sprite_sheet;
    anim->layout_count = count;
    anim->layouts = (SpriteSheetAnimationLayout**)MemAlloc(sizeof(SpriteSheetAnimationLayout*) * count, MEMORY_TAG_STRUCT);
        
    for (i32 i = 0; i < anim->layout_count; i++)
    {
        anim->layouts[i] = (SpriteSheetAnimationLayout*)MemAlloc(sizeof(SpriteSheetAnimationLayout), MEMORY_TAG_STRUCT);
        SpriteSheetAnimationLayout* layout = anim->layouts[i];
        layout->name = layouts[i].name;
        layout->start_frame = layouts[i].start_frame;
        layout->end_frame = layouts[i].end_frame;
        layout->total_time = layouts[i].total_time;

        layout->current_frame = layout->start_frame;
        layout->frames_per_second = ((float)layout->end_frame - (float)layout->start_frame) / layout->total_time;
        layout->time_step = 1.0f / layout->frames_per_second;
    }

    return anim;
}

void DeleteSpriteSheetAnimation(SpriteSheetAnimation* ss_animation)
{
    for (i32 i = 0; i < ss_animation->layout_count; i++)
    {
        MemFree(ss_animation->layouts[i] ,sizeof(SpriteSheetAnimationLayout), MEMORY_TAG_STRUCT);
    }

    MemFree(ss_animation->layouts ,sizeof(SpriteSheetAnimationLayout*) * ss_animation->layout_count, MEMORY_TAG_STRUCT);
    MemFree(ss_animation, sizeof(SpriteSheetAnimation) , MEMORY_TAG_STRUCT);
}

void UpdateSpriteSheetAnimation(SpriteSheetAnimation* ss_animation)
{
    for (i32 i = 0; i < ss_animation->layout_count; i++)
    {
        SpriteSheetAnimationLayout* layout = ss_animation->layouts[i];
        auto dt = GetDeltaTimeMs();
        layout->time_accumulator += (float) dt;
        if(layout->time_accumulator >= layout->time_step)
        {
            layout->current_frame++;
            if(layout->current_frame >= layout->end_frame)
                layout->current_frame = layout->start_frame;
            layout->time_accumulator = 0;
        }
    }
}