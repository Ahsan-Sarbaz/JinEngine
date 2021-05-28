#include "MainLayer.h"
#include "Jin.h"

MainLayer::MainLayer(Application* app)
    :Layer("MainLayer", app)
{

}

void MainLayer::OnStart()
{  
    model = new Model("res/models/bunny.fbx");
}

void MainLayer::OnUpdate()
{   
    app->GetRenderer()->DrawModel(model);
}

void MainLayer::OnEnd() 
{
    delete model;
}
