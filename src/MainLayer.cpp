#include "MainLayer.h"
#include "Jin.h"


void MainLayerStart(Application* app)
{

}

void MainLayerUpdate(Application* app)
{   
    StartNewBatch(app->renderer);
   
    DrawCurrentBatch(app->renderer);
}

void MainLayerEnd(Application* app)
{

}

Layer* GetMainLayer(Application* app)
{
    LayerConfiguration config;
    config.name = "Main Layer";
    config.onStart = MainLayerStart;
    config.onUpdate = MainLayerUpdate;
    config.onEnd = MainLayerEnd;

    return CreateLayer(config);
}