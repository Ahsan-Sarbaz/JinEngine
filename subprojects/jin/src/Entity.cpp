#include "Entity.h"

static entt::registry s_registry = entt::registry();

Entity CreateEntity(const char* name)
{
    auto e = s_registry.create();
    s_registry.emplace<TagComponent>(e, name);
    return { e };
}

void DeleteEntity(Entity entity)
{
    s_registry.destroy(entity.id);
}

Entity CreateSpriteEntity(const char* name, Texture* t, const TransformComponent& transform)
{
    auto e = CreateEntity(name);
    s_registry.emplace<SpriteComponent>(e.id, t);
    s_registry.emplace<TransformComponent>(e.id, transform);
    return e;
}

Entity CreateSpriteSheetAnimationEntity(const char* name, SpriteSheetAnimation* anim, const TransformComponent& transform)
{
    auto e = CreateEntity(name);
    s_registry.emplace<TransformComponent>(e.id, transform);
    s_registry.emplace<SpriteSheetAnimationComponent>(e.id, anim);
    return e;
}

entt::registry&& GetEnTTRegistry()
{
    return std::move(s_registry);
}

