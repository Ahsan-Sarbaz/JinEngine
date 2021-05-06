#pragma once
#include "Types.h"

struct Texture
{
    u32 id;
    i32 width, height, channels;
    const char* name;
};

Texture* CreateTexture(const char* path);
Texture* CreateTextureFromBuffer(unsigned char* buffer, i32 width, i32 height, i32 channels);
void DeleteTexture(Texture* texture);
void BindTexture(Texture* texture, i32 unit = 0);
void UnbindTexture();