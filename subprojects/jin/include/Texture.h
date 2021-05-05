#pragma once
#include "Types.h"

struct Texture
{
    u32 id;
    i32 width, height, channels;
};

Texture* CreateTexture(const char* path);
void DeleteTexture(Texture* texture);