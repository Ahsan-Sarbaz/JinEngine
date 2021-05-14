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

    
    Application* app = CreateApplication(config);
    if(app == nullptr)
    {
        LOG_FATAL("Failed to create Application!\n");
        return -1;
    }

    LOG_INFO("Application creation Successful!\n");

    ApplicationAttachLayer(app, GetImGUILayer(app));
    ApplicationAttachLayer(app, GetMainLayer(app));

    RunApplication(app);
    DeleteApplication(app);

    auto memory_state = GetMemoryState();
    if(memory_state->total_memory > 0)
    {
        LOG_WARN("There is a memory leak\n");
        for(i32 i = 0; i < MEMORY_TAG_COUNT; ++i)
        {
            if(memory_state->memory_tag[i] > 0)
            {
                LOG_WARN("Leak in %s of size %lld\n", MemoryTagToString((MEMORY_TAG)i), memory_state->memory_tag[i]);
            }
        }
        return -2;
    }

    return 0;
}