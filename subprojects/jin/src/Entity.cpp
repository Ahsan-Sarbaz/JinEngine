#include "Entity.h"

static entt::registry s_registry = entt::registry();

Entity::Entity(const char* name)
{
    auto e = s_registry.create();
    s_registry.emplace<TagComponent>(e, name);
}

Entity::~Entity()
{
    s_registry.destroy(id);
}

Entity Entity::CreateSpriteEntity(const char* name, Texture* t, const TransformComponent& transform)
{
    auto e = Entity(name);
    s_registry.emplace<SpriteComponent>(e.GetId(), t);
    s_registry.emplace<TransformComponent>(e.GetId(), transform);
    return e;
}

Entity Entity::CreateSpriteSheetAnimationEntity(const char* name, SpriteSheetAnimation* anim, const TransformComponent& transform)
{
    auto e = Entity(name);
    s_registry.emplace<TransformComponent>(e.GetId(), transform);
    s_registry.emplace<SpriteSheetAnimationComponent>(e.GetId(), anim);
    return e;
}

entt::registry&& GetEnTTRegistry()
{
    return std::move(s_registry);
}

