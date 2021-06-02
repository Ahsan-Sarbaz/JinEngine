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
    cube->AddComponent<ModelComponent>(model);
}

void MainLayer::OnUpdate()
{   
//    Application::GetInstance()->GetRenderer()->DrawModel(model);
    ImGuiDrawLevelPanel();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("ViewPort", 0);
    static ImVec2 lastAvail = ImVec2{0,0};
    auto avail = ImGui::GetContentRegionAvail();
    if(avail.x != lastAvail.x || avail.y != lastAvail.y)
    {
        Application::GetInstance()->GetRenderTarget()->Resize((u32)avail.x, (u32)avail.y);
        lastAvail = avail;
    }
    ImGui::Image((ImTextureID)Application::GetInstance()->GetRenderTarget()->GetAttachmentId(0), avail, { 0,1 }, {1,0});
    ImGui::End();
    ImGui::PopStyleVar();
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
