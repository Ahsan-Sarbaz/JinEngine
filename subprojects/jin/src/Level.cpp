#include "Level.h"
#include "Entity.h"

Level::Level(const char* _name , const std::filesystem::path& path)
    :name(_name), assets_path(path)
{
    root = new Entity(registry.create(), this);
    root->AddComponent<TagComponent>("root");
    root->AddComponent<TransformComponent>();
}

Entity* Level::CreateEntity(const char *name)
{
    auto e = AddChild(root, name);
    return e;
}

void Level::RemoveEntity(Entity *e)
{
    if(e->GetParent() != nullptr)
    {
        e->GetParent()->RemoveChild(e);
        //registry.destroy(e->GetHandle());
    }
}

Entity* Level::AddChild(Entity *parent, const char* name)
{
    if(parent == nullptr) parent = root;
    auto child = parent->AddChild(new Entity(registry.create(), this));
    child->AddComponent<TagComponent>(name);
    child->AddComponent<TransformComponent>();
    return child;
}
