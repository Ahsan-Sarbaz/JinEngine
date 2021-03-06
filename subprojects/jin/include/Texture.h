#pragma once
#include "Types.h"

class Texture
{
private:
    u32 id;
    i32 unit;
    i32 width, height, channels;
    
public:
    Texture() = default;
    void InitFromFile(const path& path);
    void InitFromBuffer(unsigned char* buffer, i32 width, i32 height, i32 channels);
    void Bind(i32 unit = 0);
    void Unbind();

    inline u32 GetId() { return id; }
    inline u32 GetWidth() { return width;}
    inline u32 GetHeight() { return height;}
    inline u32 GetChannels() { return channels;}
};

