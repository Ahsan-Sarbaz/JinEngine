#pragma once
#include "Types.h"
#include <vector>

enum FramebufferAttachmentFormat
{
    FRAMEBUFFER_ATTACHMENT_FORMAT_RGBA8,
    FRAMEBUFFER_ATTACHMENT_FORMAT_RED_INT,
    FRAMEBUFFER_ATTACHMENT_FORMAT_DEPTH
};

struct FramebufferAttachmentConfiguration
{
    FramebufferAttachmentFormat format;
};

struct FramebufferConfiguration
{
    u32 width, height;
    std::vector<FramebufferAttachmentConfiguration> attachmentConfigs;
    u32 samples;
};

class Framebuffer
{
private:
    u32 id;
    FramebufferConfiguration config;
    std::vector<u32> attachments;

public:
    Framebuffer() = default;
    Framebuffer(const FramebufferConfiguration& config);

    void Bind();
    void Unbind();
    void Resize(u32 width, u32 height);
    void Invalidate();
    b8 IsComplete();
    u32 GetAttachmentId(u32 index) { return attachments[index]; }
    inline FramebufferConfiguration* GetConfig() { return &config; }
};

