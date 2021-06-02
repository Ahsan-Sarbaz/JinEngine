#pragma once
#include "Types.h"
#include "CubeMap.h"
#include "entt.h"

class Entity;
class Application;

class Level
{
private:
    const char* name;
    CubeMap* skybox {nullptr};
    entt::registry registry;
    Entity* root;

public:
    Level(const char* name);
    const char* GetName() { return name; }
    void SetName(const char* _name) { name = _name; }

    inline Entity* GetRootEntity() { return root; }

    Entity* CreateEntity(const char* name);
    void RemoveEntity(Entity* e);
    Entity* AddChild(Entity* parent, const char* name);

    inline void SetSkyBox(CubeMap* map) { skybox = map; }
    
    inline CubeMap* GetSkyBox() { return skybox; }

    friend class Entity;
    friend class Application;
};
