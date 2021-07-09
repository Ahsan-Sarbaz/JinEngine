#include "Texture.h"
#include "Logger.h"
#include "IO.h"

#include <GL/glew.h>

void Texture::InitFromFile(const path& path)
{
    u8* buffer;
    i32 size = 0;
    i32 width = 0 , height = 0, channels = 0;
    auto success = ReadTextureToBuffer(path, &buffer, &size, &width, &height, &channels);
    if(!success)
    {
        std::string pathString = path.string();
        LOG_ERROR("Failed to load texture %s\n", pathString.c_str());
        return;
    }
    InitFromBuffer(buffer, width, height, channels);
    FreeTextureBuffer(&buffer, size);
}

void Texture::InitFromBuffer(unsigned char* buffer, i32 width, i32 height, i32 channels)
{
    width = width;
    height = height;
    channels = channels;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLenum internalFormat = 0;
    GLenum format = 0;

    if(channels == 1)
    {
        internalFormat = GL_RED;
        format = GL_RED;
    }
    else if(channels == 3)
    {
        internalFormat = GL_RGB8;
        format = GL_RGB;
    }
    else if(channels == 4)
    {
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, buffer);

    glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::Bind(i32 _unit)
{
    unit = _unit;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind()
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}
