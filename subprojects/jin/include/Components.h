#pragma once
#include "Texture.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "SpriteSheet.h"
#include "SpriteSheetAnimation.h"


//TODO: use pointer to stuff like textures etc or to create in place

struct TagComponent
{
    const char* name;
};

struct TransformComponent
{
    Vector3 position;
    Vector3 rotation;
    Vector3 size;
};

struct SpriteComponent
{
    Texture* texture;
};

struct SpriteSheetAnimationComponent
{
    SpriteSheetAnimation* spriteSheetAnimation;
};