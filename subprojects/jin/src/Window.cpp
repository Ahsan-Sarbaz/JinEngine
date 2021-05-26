#include "Window.h"
#include "Logger.h"

#include <GLFW/glfw3.h>

void Window::Init(const WindowConfiguration& _config)
{
    config = _config;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.gl_contex_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.gl_contex_version_minor);
    if(config.enable_context_core_profile)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(config.fullscreen)
    {
        monitor = glfwGetPrimaryMonitor();
        glfwGetMonitorWorkarea(monitor,0,0, &config.width, &config.height);
    }
    
    handle = glfwCreateWindow(config.width, config.height, config.title, monitor, 0);
    if(handle == nullptr)
    {
        return;
    }

    glfwMakeContextCurrent(handle);
    glfwSwapInterval(config.vsync ? 1 : 0);
}

// void DeleteWindow(Window* window)
// {
//     glfwDestroyWindow(window->handle);
//     MemFree(window, sizeof(Window), MEMORY_TAG_STRUCT);
// }
