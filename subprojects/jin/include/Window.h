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

class Window
{
    private:
    WindowConfiguration config;
    GLFWwindow* handle;
    GLFWmonitor* monitor;

    public:
    Window() = default;

    void Init(const WindowConfiguration& config);

    WindowConfiguration* GetConfig() { return &config; }
    GLFWwindow* GetHandle() { return handle; }
    GLFWmonitor* GetMonitor() { return monitor; }
};
