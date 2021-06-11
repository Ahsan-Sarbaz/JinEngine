#include "CubeMap.h"

#include "IO.h"
#include "Logger.h"

#include <GL/glew.h>
#include <stb_image.h>

CubeMap::CubeMap(const char** path)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    i32 width, height;

    for (u32 i = 0; i < 6; i++)
    {
        u8* buffer;
        i32 size = 0;
        i32 width = 0 , height = 0, channels = 0;
        auto success = ReadTextureToBuffer(path[i], &buffer, &size, &width, &height, &channels);
        if(!success)
        {
            LOG_ERROR("Failed to read file %s\n", path[i]);
            return;
        }
        GLenum internalFormat = 0;
        GLenum format = 0;
        if(channels == 3)
        {
            internalFormat = GL_RGB8;
            format = GL_RGB;
        }
        else if(channels == 4)
        {
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, buffer);

        FreeTextureBuffer(&buffer, size);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void CubeMap::Unbind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
