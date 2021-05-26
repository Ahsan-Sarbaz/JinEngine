#include "MainLayer.h"
#include "Jin.h"
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model* model;

void MainLayerStart(Application* app)
{  
    model = new Model();
    model->InitFromFile("res/models/Car.dae");
}

void MainLayerUpdate(Application* app)
{   
    app->GetRenderer()->DrawModel(model);
}

void MainLayerEnd(Application* app)
{
    delete model;
}

Layer* GetMainLayer(Application* app)
{
    LayerConfiguration config;
    config.name = "Main Layer";
    config.onStart = MainLayerStart;
    config.onUpdate = MainLayerUpdate;
    config.onEnd = MainLayerEnd;

    auto layer = new Layer();
    layer->Init(config);
    return layer;
}