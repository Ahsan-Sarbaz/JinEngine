#pragma once
#include "entt.h"
#include "Texture.h"
#include "SpriteSheetAnimation.h"
#include "Components.h"

struct Entity
{
    entt::entity id;
};

Entity CreateEntity(const char* name);
void DeleteEntity(Entity entity);
Entity CreateSpriteEntity(const char* name, Texture* t, const TransformComponent& transform);
Entity CreateSpriteSheetAnimationEntity(const char* name, SpriteSheetAnimation* anim, const TransformComponent& transform);
// TODO: think about pointer to registry

entt::registry&& GetEnTTRegistry();