#include "Texture.h"
#include "Logger.h"
#include "Memory.h"
#include "IO.h"

#include <GL/glew.h>

Texture* CreateTexture(const char* path)
{
    u8* buffer;
    i32 size = 0;
    i32 width = 0 , height = 0, channels = 0;
    auto success = ReadTextureToBuffer(path, &buffer, &size, &width, &height, &channels);

    Texture* texture = CreateTextureFromBuffer(buffer, width, height, channels);
    texture->name = path;
    // if(success)
    // {
    //     LOG_INFO("Loaded Texture %s Successfully %dx%dx%d\n", path, texture->width, texture->height, texture->channels);
    // }

    FreeTextureBuffer(&buffer, size);
    return texture;
}

Texture* CreateTextureFromBuffer(unsigned char* buffer, i32 width, i32 height, i32 channels)
{
    Texture* texture = (Texture*)MemAlloc(sizeof(Texture), MEMORY_TAG_STRUCT);
    texture->width = width;
    texture->height = height;
    texture->channels = channels;
    texture->name = "May Be White";

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum internalFormat = 0;
    GLenum format = 0;
    if(texture->channels == 3)
    {
        internalFormat = GL_RGB8;
        format = GL_RGB;
    }
    else if(texture->channels == 4)
    {
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, buffer);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}


void DeleteTexture(Texture* texture)
{
    if(texture)
        glDeleteTextures(1, &texture->id);
    MemFree(texture, sizeof(Texture), MEMORY_TAG_STRUCT);
}

void BindTexture(Texture* texture, i32 unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void UnbindTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}