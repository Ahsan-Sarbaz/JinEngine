#pragma once
#include "Types.h"

struct GLFWwindow;

struct WindowConfiguration
{
    u32 width, height;
    const char* title;
    b32 fullscreen, vsync;
    u8 gl_contex_version_major;
    u8 gl_contex_version_minor;
    b8 enable_context_core_profile;
};

struct Window
{
    WindowConfiguration config;
    GLFWwindow* handle;
};

Window* CreateWindow(const WindowConfiguration& config);
void DeleteWindow(Window* window);
