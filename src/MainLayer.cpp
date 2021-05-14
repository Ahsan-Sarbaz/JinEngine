#include "MainLayer.h"
#include "Jin.h"

void MainLayerStart(Application* app)
{
    RegisterListener(EVENT_TYPE_KEYBOARD_KEY_DOWN, [](Event e){
        if(e.data.key_char == JIN_KEY_SPACE)
        {
            if(e.data.key_mods & (JIN_MOD_CONTROL))
            {
                LOG_FATAL("SPACE PRESSED %d %d\n", e.data.key_char, e.data.key_mods);
            }
        }
    });

    RegisterListener(EVENT_TYPE_WINDOW_RESIZE, [](Event e){
        LOG_TRACE("Window Resize %dx%d\n", e.data.signed_int[0],e.data.signed_int[1]);
    });

    RegisterListener(EVENT_TYPE_MOUSE_MOVE, [](Event e){
        LOG_TRACE("Mouse Move %.2f,%.2f\n", e.data.real_double[0],e.data.real_double[1]);
    });

    RegisterListener(EVENT_TYPE_MOUSE_BUTTON_DOWN, [](Event e){
        LOG_TRACE("Mouse Button Down %d\n", e.data.signed_int[0]);
    });
}

void MainLayerUpdate(Application* app)
{   

}

void MainLayerEnd(Application* app)
{

}

Layer* GetMainLayer(Application* app)
{
    LayerConfiguration config;
    config.name = "Main Layer";
    config.onStart = MainLayerStart;
    config.onUpdate = MainLayerUpdate;
    config.onEnd = MainLayerEnd;

    return CreateLayer(config);
}