#include "MainLayer.h"
#include "Jin.h"
#include "imgui.h"

MainLayer::MainLayer()
    :Layer("MainLayer")
{

}

void MainLayer::OnStart()
{  
    // model = new Model("res/models/bunny.fbx");
    model = new Model(Mesh::CreateCubeMesh());

    const char* skyPaths[6] = {
        "res/textures/sky/posx.jpg",
        "res/textures/sky/negx.jpg",
        "res/textures/sky/posy.jpg",
        "res/textures/sky/negy.jpg",
        "res/textures/sky/posz.jpg",
        "res/textures/sky/negz.jpg",
    };

    Application::GetInstance()->GetEditorCamera()->SetPosition({0.0f,0.0f, 5.0f});
    skyMap = new CubeMap(skyPaths);

    level = new Level("Main Level");
    level->SetSkyBox(skyMap);

    Application::GetInstance()->SetCurrentLevel(level);

    auto cube = level->CreateEntity("cube");
    auto anotherCube = level->AddChild(cube, "anotherCube");
    auto yetAnotherCube = level->AddChild(cube, "yetAnotherCube");

    auto cube1 = level->CreateEntity("cube1");
    auto anotherCube1 = level->AddChild(cube1, "anotherCube1");
    auto yetAnotherCube1 = level->AddChild(anotherCube1, "yetAnotherCube1");
}

void MainLayer::OnUpdate()
{   
    Application::GetInstance()->GetRenderer()->DrawModel(model);
    ImGuiDrawLevelPanel();
}

void MainLayer::OnEnd() 
{
    delete level;
    delete model;
    delete skyMap;
}

void MainLayer::ImGuiDrawLevelPanel()
{
    auto lvl = Application::GetInstance()->GetCurrentLevel();
    ImGui::Begin("Level");
    ImGui::Text("Level: %s", lvl->GetName());
    ImGuiDrawEntity(lvl->GetRootEntity());
    ImGui::End();
}

void MainLayer::ImGuiDrawEntity(Entity* entity)
{
    auto tag = entity->GetComponent<TagComponent>();

    if(entity->HasChildren())
    {
        if(ImGui::TreeNode((void*)((u32)entity->GetHandle() + 786786), "%s", tag.name))
        {
            for (u32 i = 0; i < entity->GetChildCount(); ++i)
            {
                ImGuiDrawEntity(entity->GetChild(i));
            }
            ImGui::TreePop();
        }
    }
    else
    {
        ImGui::Text("%s", tag.name);
    }
}
