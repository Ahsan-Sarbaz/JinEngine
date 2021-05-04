#pragma once
#include "Types.h"

struct GLFWwindow;
struct GLFWmonitor;

struct WindowConfiguration
{
    i32 width, height;
    const char* title;
    b8 fullscreen, vsync;
    u8 gl_contex_version_major;
    u8 gl_contex_version_minor;
    b8 enable_context_core_profile;
};

struct Window
{
    WindowConfiguration config;
    GLFWwindow* handle;
    GLFWmonitor* monitor;
};

Window* CreateWindow(const WindowConfiguration& config);
void DeleteWindow(Window* window);
