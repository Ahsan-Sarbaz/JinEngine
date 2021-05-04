#pragma once
#include "Types.h"
#include "Window.h"
#include "Layer.h"


struct Renderer;

struct ApplicationConfiguration
{
    WindowConfiguration windowConfig;
    u8 opengl_version_major;
    u8 opengl_version_minor;
};


struct Application
{
    ApplicationConfiguration config;
    Window* window;
    Layer* layers[32];
    u32 layersCount;
    Renderer* renderer;    
};

Application* CreateApplication(const ApplicationConfiguration& config);
void DeleteApplication(Application* app);
void RunApplication(Application* app);
void ApplicationAttachLayer(Application* app, Layer* layer);