#pragma once
#include "Layer.h"

class ImGuiLayer: public Layer
{
private:

public:
    ImGuiLayer(Application* app);
    virtual void OnStart() override;
    virtual void OnUpdate() override;
    virtual void OnEnd() override;
};