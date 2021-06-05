#include "MainLayer.h"
#include "Jin.h"
#include "imgui.h"

MainLayer::MainLayer()
    :Layer("MainLayer")
{
    camera = new EditorCamera();

    EventListener editorCameraKeyboardEventListener = {};
    editorCameraKeyboardEventListener.object = camera;
    editorCameraKeyboardEventListener.type = EVENT_TYPE_KEYBOARD_KEY_REPEAT | EVENT_TYPE_KEYBOARD_KEY_DOWN;
    editorCameraKeyboardEventListener.callback = [](void* object, Event e){
        auto camera = (EditorCamera*)object;
        float last_speed = camera->GetMovementSpeed();
        switch(e.data.key_char)
        {
        case JIN_KEY_W:
        case JIN_KEY_UP:
            if(e.data.key_mods == JIN_MOD_SHIFT) camera->SetMovementSpeed(10.0f);
            camera->ProcessKeyboard(CAMERA_MOVEMENT_FORWARD);
            if(e.data.key_mods == JIN_MOD_SHIFT) camera->SetMovementSpeed(last_speed);
            break;
        case JIN_KEY_S:
        case JIN_KEY_DOWN:
            if(e.data.key_mods == JIN_MOD_SHIFT) camera->SetMovementSpeed(10.0f);
            camera->ProcessKeyboard(CAMERA_MOVEMENT_BACKWARD);
            if(e.data.key_mods == JIN_MOD_SHIFT) camera->SetMovementSpeed(last_speed);
            break;
        case JIN_KEY_A:
        case JIN_KEY_LEFT:
            if(e.data.key_mods == JIN_MOD_SHIFT) camera->SetMovementSpeed(10.0f);
            camera->ProcessKeyboard(CAMERA_MOVEMENT_LEFT);
            if(e.data.key_mods == JIN_MOD_SHIFT) camera->SetMovementSpeed(last_speed);
            break;
        case JIN_KEY_D:
        case JIN_KEY_RIGHT:
            if(e.data.key_mods == JIN_MOD_SHIFT) camera->SetMovementSpeed(10.0f);
            camera->ProcessKeyboard(CAMERA_MOVEMENT_RIGHT);
            if(e.data.key_mods == JIN_MOD_SHIFT) camera->SetMovementSpeed(last_speed);
            break;
        }
    };
    Application::AddEventListener(editorCameraKeyboardEventListener);
    EventListener editorCameraMouseEventListener = {};
    editorCameraMouseEventListener.object = camera;
    editorCameraMouseEventListener.type = EVENT_TYPE_MOUSE_MOVE;
    editorCameraMouseEventListener.callback = [](void* object, Event e) {
        auto camera = (EditorCamera*)object;
        static bool firstMouse = true;
        static float lastX = 0;
        static float lastY = 0;
        if ( firstMouse )
        {
            lastX = e.data.x;
            lastY = e.data.y;
            firstMouse = false;
        }

        float xOffset = e.data.x - lastX;
        float yOffset = lastY - e.data.y;  // Reversed since y-coordinates go from bottom to left

        lastX = e.data.x;
        lastY = e.data.y;
        camera->ProcessMouseMove(xOffset, yOffset);
    };

    Application::AddEventListener(editorCameraMouseEventListener);
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

    camera->SetPosition({0.0f,0.0f, 5.0f});
    skyMap = new CubeMap(skyPaths);

    level = new Level("Main Level");
    level->SetSkyBox(skyMap);

    Application::SetCurrentLevel(level);

    auto cube = level->CreateEntity("cube");
    cube->AddComponent<ModelComponent>(model);
}

void MainLayer::OnUpdate()
{
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

    Renderer::CameraUBOLayout cameraUboLayout = {};
    cameraUboLayout.projection = camera->GetProjectionMatrix((float)avail.x, (float)avail.y);
    cameraUboLayout.view = camera->GetViewMatrix();
    cameraUboLayout.position = camera->GetPosition();
    Renderer::SetCameraUBOData(cameraUboLayout);

    ImGui::Image((ImTextureID)Application::GetInstance()->GetRenderTarget()->GetAttachmentId(0), avail, { 0,1 }, {1,0});
    ImGui::End();
    ImGui::PopStyleVar();

}

void MainLayer::OnEnd() 
{
    delete level;
    delete model;
    delete skyMap;
    delete camera;
}

void MainLayer::ImGuiDrawLevelPanel()
{
    auto lvl = Application::GetInstance()->GetCurrentLevel();
    ImGui::Begin("Level");
    ImGui::Text("Level: %s", lvl->GetName());
    ImGuiDrawEntity(lvl->GetRootEntity());

    if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        selectedEntity = nullptr;

    if(ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if(ImGui::MenuItem("Create Empty Entity"))
        {
            lvl->AddChild(selectedEntity, "Empty Entity");
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

void MainLayer::ImGuiDrawEntity(Entity* entity)
{
    auto lvl = Application::GetCurrentLevel();
    auto tag = entity->GetComponent<TagComponent>();

    ImGuiTreeNodeFlags flags = ((selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0);
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;

    if(!entity->HasChildren())
    {
        flags |= ImGuiTreeNodeFlags_Bullet;
    }

    bool opened = ImGui::TreeNodeEx((void*)entity, flags, tag.name);

    if(ImGui::IsItemFocused())
    {
        selectedEntity = entity;
    }
    bool entityDeleted = false;
    bool addChild = false;

    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Add Child"))
            addChild = true;

        if (ImGui::MenuItem("Delete Entity"))
            entityDeleted = true;

        ImGui::EndPopup();
    }

    if(entityDeleted)
    {
        if(entity == lvl->GetRootEntity())
        {
            if(opened)
            {
                ImGui::TreePop();
            }
            return;
        }
        lvl->RemoveEntity(entity);
        selectedEntity = nullptr;
    }

    if(addChild)
    {
        selectedEntity = lvl->AddChild(selectedEntity, "Empty Entity");
    }

    if(opened)
    {
        for (u32 i = 0; i < entity->GetChildCount(); ++i)
        {
            ImGuiDrawEntity(entity->GetChild(i));
        }
        ImGui::TreePop();
    }
}


void MainLayer::ImGuiDrawComponentsPanel()
{
    if(ImGui::Begin("Components"))
    {
        if(selectedEntity)
        {
            auto& tag = selectedEntity->GetComponent<TagComponent>();
            ImGui::Text("Name: %s", tag.name);
        }
        ImGui::End();
    }
}
