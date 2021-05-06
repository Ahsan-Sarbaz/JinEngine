#include "SpriteSheet.h"
#include "Logger.h"
#include "Memory.h"

SpriteSheet* CreateSpriteSheet(const SpriteSheetConfiguration& config)
{
    SpriteSheet* spriteSheet = (SpriteSheet*)MemAlloc(sizeof(SpriteSheet), MEMORY_TAG_STRUCT);
    spriteSheet->config = config;
    
    spriteSheet->sprites_per_row = spriteSheet->config.texture->width / spriteSheet->config.sprite_width;
    spriteSheet->sprites_per_column = spriteSheet->config.texture->height / spriteSheet->config.sprite_height;

    float tw = (float)spriteSheet->config.sprite_width / (float)spriteSheet->config.texture->width;
    float th = (float)spriteSheet->config.sprite_height / (float)spriteSheet->config.texture->height;

    spriteSheet->sprites_count = spriteSheet->sprites_per_row * spriteSheet->sprites_per_column;

    spriteSheet->rects = (RectF*)MemAlloc(sizeof(RectF) * spriteSheet->sprites_count, MEMORY_TAG_STRUCT);

    for (u32 x = 0; x < spriteSheet->sprites_per_row; x++)
    {
        for (u32 y = 0; y < spriteSheet->sprites_per_column; y++)
        {
            auto rect = RectF{ 
                (float) x * tw,
				(float) y * th,
				((float)x * tw) + tw,
				((float)y * th) + th
			};

            // if(y * spriteSheet->sprites_per_row + x > spriteSheet->sprites_count)
            //     LOG_TRACE("We got a problem\n");
            spriteSheet->rects[y * spriteSheet->sprites_per_row + x] = rect;
        }
    }
    

    return spriteSheet;
}

void DeleteSpriteSheet(SpriteSheet* spriteSheet)
{
    MemFree(spriteSheet->rects, sizeof(RectF) * spriteSheet->sprites_count, MEMORY_TAG_STRUCT);
    MemFree(spriteSheet, sizeof(SpriteSheet), MEMORY_TAG_STRUCT);
}