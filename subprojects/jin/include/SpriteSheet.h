#pragma once
#include "Types.h"
#include "Texture.h"
#include "Rect.h"

struct SpriteSheetConfiguration
{
    Texture* texture;
    u32 sprite_width, sprite_height;
    u32 padding;
};


struct SpriteSheet
{
    SpriteSheetConfiguration config;
    u32 sprites_per_column;
    u32 sprites_per_row;
    u32 sprites_count;
    RectF* rects;
};

SpriteSheet* CreateSpriteSheet(const SpriteSheetConfiguration& config);
void DeleteSpriteSheet(SpriteSheet* spriteSheet);
