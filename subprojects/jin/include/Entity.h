#pragma once
#include "Components.h"
#include "SpriteSheetAnimation.h"
#include "Texture.h"
#include "entt.h"
#include <list>
#include "Level.h"
#include "Logger.h"

class Entity
{
  private:
    entt::entity handle;
    Level* level;
    Entity *parent{nullptr};
    std::list<Entity *> children;

  public:
    Entity() = default;
    Entity(entt::entity handle, Level* level);

    inline void SetParent(Entity *entity)
    {
        parent = entity;
    }

    inline entt::entity GetHandle()
    {
        return handle;
    }
    inline Entity* GetParent()
    {
        return parent;
    }
    inline b8 HasParent()
    {
        return parent == nullptr ? TRUE : FALSE;
    }
    inline b8 HasChildren()
    {
        return children.size() > 0 ? TRUE : FALSE;
    }
    inline u32 GetChildCount()
    {
        return children.size();
    }
    inline Entity* GetChild(u32 index)
    {
        std::list<Entity *>::iterator it = children.begin();
        std::advance(it, index);
        return (*it);
    }

    template<typename T, typename... Args>
    T& AddComponent(Args&... args)
    {
        T& component = level->registry.emplace_or_replace<T>(handle, std::forward<Args>(args)...);
        //TODO: on component added
        return component;
    }

    template<typename T>
    void RemoveComponent()
    {
        level->registry.remove_if_exists<T>(handle);
    }
    template<typename T>
    T& GetComponent()
    {
        auto comp = level->registry.try_get<T>(handle);
        if(comp == nullptr) { LOG_ERROR("Component does not exist\n"); }
        return *comp;
    }

private:
    inline Entity* AddChild(Entity *entity)
    {
        entity->SetParent(this);
        return children.emplace_back(entity);
    }
    inline void RemoveChild(Entity *entity)
    {
        children.remove(entity);
    }

    friend class Level;
};

entt::registry &&GetEnTTRegistry();
