#pragma once
#include "Jin.h"
#include <filesystem>
#include <unordered_map>

class MainLayer : public Layer
{
private:
    Model* model;
    CubeMap* skyMap;
    Level* level;
    EditorCamera* camera;
    Entity* selectedEntity;
    u32 guizmoType;
    b8 guizmoUsed;
    std::filesystem::path contentBrowserPath;
    Texture* contentBrowserDirectoryIcon;
    Texture* contentBrowserFileIcon;
    std::unordered_map<std::string, Texture*> thumbnails;


public:
    MainLayer();
    virtual void OnStart() override;
    virtual void OnUpdate() override;
    virtual void OnEnd() override;

    inline void SetGuizmoType(u32 type) { if(!guizmoUsed) guizmoType = type; }

private:
    void ImGuiDrawLevelPanel();
    void ImGuiDrawEntity(Entity* entity);
    void ImGuiDrawComponentsPanel();
    void ImGuiDrawContentBrowser();
};
