#pragma once
#include "Types.h"
#include "ShaderProgram.h"

class Material
{
protected:
    ShaderProgram* program;

public:
    Material() = default;
    Material(ShaderProgram* program);

    inline ShaderProgram* GetProgram() { return program; }
};