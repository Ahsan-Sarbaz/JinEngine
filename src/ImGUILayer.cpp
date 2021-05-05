#include "ImGUILayer.h"
#include "Jin.h"
#include "imgui.h"

Texture* t;

void ImGUILayerStart(Application* app)
{
    t = CreateTexture("res/textures/test.png");
}

void ImGUILayerUpdate(Application* app)
{   
    if(ImGui::Begin("ImGui Layer"))
    {
        ImGui::Image((ImTextureID)t->id, {256, 256});
        ImGui::End();
    }
}

void ImGUILayerEnd(Application* app)
{
    DeleteTexture(t);
}

Layer* GetImGUILayer(Application* app)
{
    LayerConfiguration config;
    config.name = "ImGUI Layer";
    config.onStart = ImGUILayerStart;
    config.onUpdate = ImGUILayerUpdate;
    config.onEnd = ImGUILayerEnd;

    return CreateLayer(config);
}