#pragma once
#include "Types.h"
#include "CubeMap.h"
#include "entt.h"
#include <filesystem>

class Entity;
class Application;

class Level
{
private:
    const char* name;
    std::filesystem::path assets_path;
    CubeMap* skybox {nullptr};
    entt::registry registry;
    Entity* root;

public:
    Level(const char* name, const std::filesystem::path& path);
    const char* GetName() { return name; }
    void SetName(const char* _name) { name = _name; }

    const std::filesystem::path& GetAssetsPath() { return assets_path; }

    inline Entity* GetRootEntity() { return root; }

    Entity* CreateEntity(const char* name);
    void RemoveEntity(Entity* e);
    Entity* AddChild(Entity* parent, const char* name);

    inline void SetSkyBox(CubeMap* map) { skybox = map; }
    
    inline CubeMap* GetSkyBox() { return skybox; }

    friend class Entity;
    friend class Application;
};
