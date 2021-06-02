#include "Framebuffer.h"
#include <GL/glew.h>
#include "Logger.h"
#include "Event.h"
#include "Application.h"

Framebuffer::Framebuffer(const FramebufferConfiguration& _config)
    :config(_config)
{
    id = 0;
    Invalidate();
}

void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(u32 width, u32 height)
{
    config.width = width;
    config.height = height;

    Event e;
    e.data.width = width;
    e.data.height = height;
    e.type = EVENT_TYPE_FRAME_BUFFER_RESIZE;
    Application::GetInstance()->AddEvent(e);
}

void Framebuffer::Invalidate()
{
    if(id)
    {
        glDeleteFramebuffers(1, &id);
        glDeleteTextures(attachments.size(), attachments.data());
        attachments.clear();
    }

    glGenFramebuffers(1, &id);
    bool multisample = config.samples > 1;

    i32 colorAttIndex = -1;
    if(config.attachmentConfigs.size())
    {
        for(u32 i = 0; i < config.attachmentConfigs.size(); ++i)
        {
            GLenum internalFormat;
            GLenum format;
            switch (config.attachmentConfigs[i].format)
            {
            case FRAMEBUFFER_ATTACHMENT_FORMAT_RGBA8:
                internalFormat = GL_RGBA8;
                format = GL_RGBA;
                colorAttIndex++;
                break;
            case FRAMEBUFFER_ATTACHMENT_FORMAT_RED_INT:
                internalFormat = GL_R32I;
                format = GL_RED_INTEGER;
                colorAttIndex++;
                break;
            case FRAMEBUFFER_ATTACHMENT_FORMAT_DEPTH:
                internalFormat = GL_DEPTH_COMPONENT;
                format = GL_DEPTH_COMPONENT;
                break;
            }
            attachments.push_back(0);
            glGenTextures(1, &attachments[i]);
            glBindTexture(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, attachments[i]);

            if(multisample)
            {
                // TODO: investigate the last GL_FALSE
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, config.samples, internalFormat, config.width, config.height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, config.width, config.height, 0, format, GL_UNSIGNED_BYTE, nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            //glBindTexture(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 0);
            Bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                                   internalFormat == GL_DEPTH_COMPONENT ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0 + colorAttIndex,
                                   multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
                                   attachments[i], 0);
            Unbind();
        }
    }

}

b8 Framebuffer::IsComplete()
{
    auto status = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER);
    switch (status)
    {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        LOG_ERROR("framebuffer attachment points are framebuffer incomplete\n");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        LOG_ERROR("framebuffer does not have at least one image attached to it\n");
        break;
    }
    return (status == GL_FRAMEBUFFER_COMPLETE);
}
