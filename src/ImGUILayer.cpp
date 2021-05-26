#include "ImGUILayer.h"
#include "Jin.h"
#include "imgui.h"


void ImGUILayerStart(Application* app)
{

}

void ImGUILayerUpdate(Application* app)
{   
}

void ImGUILayerEnd(Application* app)
{
}

Layer* GetImGUILayer(Application* app)
{
    LayerConfiguration config;
    config.name = "ImGUI Layer";
    config.onStart = ImGUILayerStart;
    config.onUpdate = ImGUILayerUpdate;
    config.onEnd = ImGUILayerEnd;

    auto layer = new Layer();
    layer->Init(config);
    return layer;
}