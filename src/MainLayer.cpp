#include "MainLayer.h"
#include "Jin.h"
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model* model;

void MainLayerStart(Application* app)
{
    model = CreateModelFromFile("res/models/Car.dae");
}


void MainLayerUpdate(Application* app)
{   
    DrawModel(model);
}

void MainLayerEnd(Application* app)
{
    DeleteModel(model);
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