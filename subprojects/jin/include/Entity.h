#pragma once
#include "entt.h"
#include "Texture.h"
#include "SpriteSheetAnimation.h"
#include "Components.h"

class Entity
{
    entt::entity id;
    
    public:
    Entity(const char* name);
    ~Entity();
    inline entt::entity GetId() { return id; }

    static Entity CreateSpriteEntity(const char* name, Texture* t, const TransformComponent& transform);
    static Entity CreateSpriteSheetAnimationEntity(const char* name, SpriteSheetAnimation* anim, const TransformComponent& transform);
};

entt::registry&& GetEnTTRegistry();