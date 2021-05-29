#pragma once
#include "Jin.h"

class MainLayer : public Layer
{
private:
    Model* model;
    CubeMap* skyMap;
public:
    MainLayer(Application* app);
    virtual void OnStart() override;
    virtual void OnUpdate() override;
    virtual void OnEnd() override;
};