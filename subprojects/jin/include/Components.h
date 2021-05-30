#pragma once
#include "Texture.h"
#include "SpriteSheet.h"
#include "SpriteSheetAnimation.h"


//TODO: use pointer to stuff like textures etc or to create in place

struct TagComponent
{
    const char* name;
};

struct TransformComponent
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct SpriteComponent
{
    Texture* texture;
};

struct SpriteSheetAnimationComponent
{
    SpriteSheetAnimation* spriteSheetAnimation;
};