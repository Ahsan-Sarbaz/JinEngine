#include "SpriteSheetAnimation.h"
#include "Logger.h"
#include "JinTime.h"

SpriteSheetAnimation::SpriteSheetAnimation(SpriteSheet* _sprite_sheet, const SpriteSheetAnimationLayout& _layout)
{
    sprite_sheet = sprite_sheet;

    layout.name = _layout.name;
    layout.start_frame = _layout.start_frame;
    layout.end_frame = _layout.end_frame;
    layout.total_time = _layout.total_time;
    
    layout.current_frame = layout.start_frame;
    layout.frames_per_second = ((float)layout.end_frame - (float)layout.start_frame) / layout.total_time;
    layout.time_step = 1.0f / layout.frames_per_second;
}

void SpriteSheetAnimation::Update()
{
    auto dt = GetDeltaTimeMs();
    layout.time_accumulator += (float) dt;
    if(layout.time_accumulator >= layout.time_step)
    {
        layout.current_frame++;
        if(layout.current_frame >= layout.end_frame)
            layout.current_frame = layout.start_frame;
        layout.time_accumulator = 0;
    }
}