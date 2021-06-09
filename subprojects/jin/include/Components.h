#pragma once
#include "Texture.h"
#include "SpriteSheet.h"
#include "SpriteSheetAnimation.h"
#include "Model.h"
#include "Material.h"
#include <string>

struct TagComponent
{
    std::string name;
    TagComponent(std::string _name)
        :name(_name)
    {

    }
};

struct TransformComponent
{
    glm::vec3 position {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation {0.0f, 0.0f, 0.0f};
    glm::vec3 scale {1.0f, 1.0f, 1.0f};
    TransformComponent() = default;
    TransformComponent(const glm::vec3& _position, const glm::vec3& _rotation, const glm::vec3& _scale)
        :position(_position), rotation(_rotation), scale(_scale)
    {

    }

    glm::mat4 GetTransform()
    {
        glm::mat4 r = glm::toMat4(glm::quat(rotation));
        return glm::translate(glm::mat4(1.0f), position) * r * glm::scale(glm::mat4(1.0f), scale);
    }
};

struct SpriteComponent
{
    Texture* texture {nullptr};
    SpriteComponent() = default;
    SpriteComponent(Texture* _texture)
        :texture(_texture)
    {

    }
};

struct SpriteSheetAnimationComponent
{
    SpriteSheetAnimation* spriteSheetAnimation {nullptr};
    SpriteSheetAnimationComponent() = default;
    SpriteSheetAnimationComponent(SpriteSheetAnimation* _animation)
        :spriteSheetAnimation(_animation)
    {

    }
};

struct ModelComponent
{
    Model* model {nullptr};
    ModelComponent() = default;
    ModelComponent(Model* _model)
        :model(_model)
    {

    }
};

struct MaterialComponent
{
    Material* material {nullptr};
    MaterialComponent() = default;
    MaterialComponent(Material* _material)
        :material(_material)
    {

    }
};
