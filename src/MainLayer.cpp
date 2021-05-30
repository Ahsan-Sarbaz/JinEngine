#include "MainLayer.h"
#include "Jin.h"

MainLayer::MainLayer(Application* app)
    :Layer("MainLayer", app)
{

}

void MainLayer::OnStart()
{  
    model = new Model("res/models/Car.dae");
    const char* skyPaths[6] = {
        "res/textures/sky/posx.jpg",
        "res/textures/sky/negx.jpg",
        "res/textures/sky/posy.jpg",
        "res/textures/sky/negy.jpg",
        "res/textures/sky/posz.jpg",
        "res/textures/sky/negz.jpg",
    };

    skyMap = new CubeMap(skyPaths);
}

void MainLayer::OnUpdate()
{   
    app->GetRenderer()->DrawModel(model);
    app->GetRenderer()->DrawSkybox(skyMap);
}

void MainLayer::OnEnd() 
{
    delete model;
    delete skyMap;
}
