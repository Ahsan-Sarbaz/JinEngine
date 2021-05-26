#pragma once
#include "Types.h"
#include "Texture.h"
#include "Rect.h"
#include <vector>

struct SpriteSheetConfiguration
{
    Texture* texture;
    u32 sprite_width, sprite_height;
    u32 padding;
};


class SpriteSheet
{
    private:
    SpriteSheetConfiguration config;
    u32 sprites_per_column;
    u32 sprites_per_row;
    u32 sprites_count;
    std::vector<RectF> rects;

    public:
    SpriteSheet() = default;
    void Init(const SpriteSheetConfiguration& config);

    inline SpriteSheetConfiguration GetConfig() { return config; }
    inline u32 GetSpritePerColumnCount() { return sprites_per_column; }
    inline u32 GetSpritePerRowCount() { return sprites_per_row; }
    //inline RectF* GetRects() { return rects; }
    inline RectF GetRect(i32 index) { return rects[index]; }
};
