#include "SpriteSheet.h"
#include "Logger.h"

void SpriteSheet::Init(const SpriteSheetConfiguration& _config)
{
    config = _config;
    
    sprites_per_row = config.texture->GetWidth() / config.sprite_width;
    sprites_per_column = config.texture->GetHeight() / config.sprite_height;

    float tw = (float)config.sprite_width / (float)config.texture->GetWidth();
    float th = (float)config.sprite_height / (float)config.texture->GetHeight();

    sprites_count = sprites_per_row * sprites_per_column;

    rects = std::vector<RectF>(sprites_count);

    for (u32 x = 0; x < sprites_per_row; x++)
    {
        for (u32 y = 0; y < sprites_per_column; y++)
        {
            auto rect = RectF{ 
                (float) x * tw,
				(float) y * th,
				((float)x * tw) + tw,
				((float)y * th) + th
			};

            // if(y * spriteSheet->sprites_per_row + x > spriteSheet->sprites_count)
            //     LOG_TRACE("We got a problem\n");
            rects[y * sprites_per_row + x] = rect;
        }
    }
}

// void DeleteSpriteSheet(SpriteSheet* spriteSheet)
// {
//     MemFree(spriteSheet->rects, sizeof(RectF) * spriteSheet->sprites_count, MEMORY_TAG_STRUCT);
//     MemFree(spriteSheet, sizeof(SpriteSheet), MEMORY_TAG_STRUCT);
// }