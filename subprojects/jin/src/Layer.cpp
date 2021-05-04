#include "Layer.h"
#include "Memory.h"

Layer* CreateLayer()
{
    Layer* layer = (Layer*)MemAlloc(sizeof(Layer), MEMORY_TAG_STRUCT);
    return layer;
}

Layer* CreateLayer(const LayerConfiguration& config)
{
    Layer* layer = (Layer*)MemAlloc(sizeof(Layer), MEMORY_TAG_STRUCT);
    layer->config = config;
    return layer;
}

void DeleteLayer(Layer* layer)
{
    MemFree(layer, sizeof(Layer), MEMORY_TAG_STRUCT);
}