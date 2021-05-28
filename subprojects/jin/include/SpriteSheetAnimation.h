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


class SpriteSheetAnimation
{
    private:
    SpriteSheet* sprite_sheet;
    SpriteSheetAnimationLayout layout;

    public:
    SpriteSheetAnimation() = default;
    SpriteSheetAnimation(SpriteSheet* sprite_sheet, const SpriteSheetAnimationLayout& layout);
    void Update();

    inline SpriteSheet* GetSpriteSheet() { return sprite_sheet; }
    inline SpriteSheetAnimationLayout GetLayout() { return layout; }
};
