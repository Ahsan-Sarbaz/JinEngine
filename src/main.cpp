#include <iostream>
#include "Jin.h"
#include "MainLayer.h"
#include "ImGUILayer.h"

int main()
{
    ApplicationConfiguration config = {};
    config.windowConfig.width = 800;
    config.windowConfig.height = 600;
    config.windowConfig.title = "Sokuban";
    config.windowConfig.vsync = TRUE;
    config.windowConfig.fullscreen = FALSE;
    config.windowConfig.enable_context_core_profile = TRUE;
    config.opengl_version_major = 3;
    config.opengl_version_minor = 3;
    config.enable_imgui = 1;

    
    Application* app = new Application();
    app->Init(config);
    if(app == nullptr)
    {
        LOG_FATAL("Failed to create Application!\n");
        return -1;
    }

    LOG_INFO("Application creation Successful!\n");

    app->AttachLayer(GetImGUILayer(app));
    app->AttachLayer(GetMainLayer(app));

    app->Run();

    delete app;

    return 0;
}