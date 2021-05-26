#pragma once
#include "Types.h"

struct Application;
typedef void (*LayerStartFunc)(Application*);
typedef void (*LayerUpdateFunc)(Application*);
typedef void (*LayerEndFunc)(Application*);

struct LayerConfiguration
{
    const char* name;
    LayerStartFunc onStart;
    LayerUpdateFunc onUpdate;
    LayerEndFunc onEnd;
};

class Layer
{
private:
    LayerConfiguration config;
public:
    Layer() = default;    
    void  Init(const LayerConfiguration& config);
    inline LayerConfiguration GetConfig() { return config; }
    inline void OnStart(Application* app) { config.onStart(app); }
    inline void OnUpdate(Application* app) { config.onUpdate(app); }
    inline void OnEnd(Application* app) { config.onEnd(app); }
};
