#pragma once
#include "Types.h"

struct Application;

class Layer
{
private:
    const char* name;

public:
    Layer() = default;
    Layer(const char* name);
    inline const char* GetName() { return name;}
    virtual void OnStart() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnEnd() = 0;
};
