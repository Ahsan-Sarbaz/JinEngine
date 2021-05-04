#include "Window.h"

#include "Memory.h"
#include "Logger.h"

#include <GLFW/glfw3.h>

Window* CreateWindow(const WindowConfiguration& config)
{
    Window* window = (Window*)MemAlloc(sizeof(Window), MEMORY_TAG_STRUCT);
    window->config = config;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.gl_contex_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.gl_contex_version_minor);
    if(config.enable_context_core_profile)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window->handle = glfwCreateWindow(config.width, config.height, config.title, 0, 0);
    if(window->handle == nullptr)
    {
        return nullptr;
    }

    glfwMakeContextCurrent(window->handle);
    glfwSwapInterval(config.vsync ? 1 : 0);
    
    return window;
}

void DeleteWindow(Window* window)
{
    glfwDestroyWindow(window->handle);
    MemFree(window, sizeof(Window), MEMORY_TAG_STRUCT);
}
