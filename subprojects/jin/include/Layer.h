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

struct Layer
{
    LayerConfiguration config;
};

Layer* CreateLayer();
Layer* CreateLayer(const LayerConfiguration& config);
void DeleteLayer(Layer* layer);
