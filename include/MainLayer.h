#pragma once
#include "Jin.h"

class MainLayer : public Layer
{
private:
    Model* model;
    CubeMap* skyMap;
    Level* level;

public:
    MainLayer();
    virtual void OnStart() override;
    virtual void OnUpdate() override;
    virtual void OnEnd() override;

private:
  void ImGuiDrawLevelPanel();
  void ImGuiDrawEntity(Entity* entity);
};
