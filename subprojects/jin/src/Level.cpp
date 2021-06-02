#include "Level.h"
#include "Entity.h"

Level::Level(const char* _name)
    :name(_name)
{
    root = new Entity(registry.create(), this);
    root->AddComponent<TagComponent>("root");
}

Entity* Level::CreateEntity(const char *name)
{
    auto e = AddChild(root, name);
    return e;
}

void Level::RemoveEntity(Entity *e)
{
    registry.destroy(e->GetHandle());
    root->RemoveChild(e);
}

Entity* Level::AddChild(Entity *parent, const char* name)
{
    auto child = parent->AddChild(new Entity(registry.create(), this));
    child->AddComponent<TagComponent>(name);
    return child;
}
