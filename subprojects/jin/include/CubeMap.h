#pragma once
#include "Types.h"

class CubeMap
{
private:
    u32 id;

public:
    CubeMap() = default;
    CubeMap(const char** path);
    void Bind();
    void Unbind();
    inline u32 GetId() { return id; }
};