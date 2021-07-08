#include "MainLayer.h"
#include "Jin.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <imguizmo.h>
#include <filesystem>


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
        switch(e.data.key_code)
        {
        case JIN_KEY_SPACE:
            camera->ResetCamera();
            break;
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
    editorCameraMouseEventListener.type = EVENT_TYPE_MOUSE_MOVE | EVENT_TYPE_MOUSE_BUTTON_DOWN;
    editorCameraMouseEventListener.callback = [](void* object, Event e) {
        auto camera = (EditorCamera*)object;
        static bool firstMouse = true;
        static float lastX = 0;
        static float lastY = 0;
        if ( firstMouse )
        {
            lastX = e.data.mouse_x;
            lastY = e.data.mouse_y;
            firstMouse = false;
        }

        float xOffset = e.data.mouse_x - lastX;
        float yOffset = lastY - e.data.mouse_y;  // Reversed since y-coordinates go from bottom to left
        float zOffset = 0;

        lastX = e.data.mouse_x;
        lastY = e.data.mouse_y;

        if(Input::IsMouseButtonDown(1))
        {
            camera->ProcessMouseMove(xOffset, yOffset, zOffset);
        }
    };

    Application::AddEventListener(editorCameraMouseEventListener);

    EventListener mainLayerKeyboardEventsListener = {};
    mainLayerKeyboardEventsListener.type = EVENT_TYPE_KEYBOARD_KEY_DOWN;
    mainLayerKeyboardEventsListener.object = this;
    mainLayerKeyboardEventsListener.callback = [](void* object, Event e) {
        auto mainLayer = (MainLayer*)object;
        switch (e.data.key_code) {
        case JIN_KEY_1:
            mainLayer->SetGuizmoType(-1);
            break;
        case JIN_KEY_2:
            mainLayer->SetGuizmoType(ImGuizmo::TRANSLATE);
            break;
        case JIN_KEY_3:
            mainLayer->SetGuizmoType(ImGuizmo::ROTATE);
            break;
        case JIN_KEY_4:
            mainLayer->SetGuizmoType(ImGuizmo::SCALE);
            break;
        }
    };

    Application::AddEventListener(mainLayerKeyboardEventsListener);
}

void MainLayer::OnStart()
{  
     model = new Model("res/models/Car.dae");
//     model = new Model(Mesh::CreateCubeMesh());

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

    level = new Level("Main Level", "res");
    level->SetSkyBox(skyMap);

    contentBrowserPath = level->GetAssetsPath();

    contentBrowserDirectoryIcon = new Texture();
    contentBrowserDirectoryIcon->InitFromFile("res/icons/cb/directory.png");

    contentBrowserFileIcon = new Texture();
    contentBrowserFileIcon->InitFromFile("res/icons/cb/file.png");


    Application::SetCurrentLevel(level);

    auto cube = level->CreateEntity("cube");
    cube->AddComponent<ModelComponent>(model);
}

void MainLayer::OnUpdate()
{
    auto lvl = Application::GetCurrentLevel();
    ImGuiDrawLevelPanel();
    ImGuiDrawComponentsPanel();
    ImGuiDrawContentBrowser();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("ViewPort", 0);

    static ImVec2 lastAvail = ImVec2{0,0};
    auto avail = ImGui::GetContentRegionAvail();
    if(avail.x != lastAvail.x || avail.y != lastAvail.y)
    {
        Application::GetRenderTarget()->Resize((u32)avail.x, (u32)avail.y);
        lastAvail = avail;
    }

    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();
    auto m_ViewportBounds1 = ImVec2{ viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
    auto m_ViewportBounds2 = ImVec2{ viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

    Renderer::CameraUBOLayout cameraUboLayout = {};
    cameraUboLayout.projection = camera->GetProjectionMatrix((float)avail.x, (float)avail.y);
    cameraUboLayout.view = camera->GetViewMatrix();
    cameraUboLayout.position = camera->GetPosition();


    ImGui::Image((ImTextureID)(u64)Application::GetInstance()->GetRenderTarget()->GetAttachmentId(0), avail, { 0,1 }, {1,0});

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(m_ViewportBounds1.x, m_ViewportBounds1.y, m_ViewportBounds2.x - m_ViewportBounds1.x, m_ViewportBounds2.y - m_ViewportBounds1.y);
    Renderer::SetCameraUBOData(cameraUboLayout);

    if (selectedEntity && guizmoType != -1 && selectedEntity != lvl->GetRootEntity())
    {
        ImGuizmo::Enable(true);

        auto& tc = selectedEntity->GetComponent<TransformComponent>();
        glm::mat4 transform = tc.GetTransform();

        // Snapping
        bool snap = false;//ImGui::IsKeyPressed(ImGuiKeyModFlags_Ctrl);
        float snapValue = 0.5f; // Snap to 0.5m for translation/scale
        // Snap to 45 degrees for rotation
        if (guizmoType == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f;

        float snapValues[3] = { snapValue, snapValue, snapValue };

        ImGuizmo::Manipulate(glm::value_ptr(cameraUboLayout.view), glm::value_ptr(cameraUboLayout.projection),
                             (ImGuizmo::OPERATION)guizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                         nullptr, snap ? snapValues : nullptr);

        guizmoUsed = ImGuizmo::IsUsing();
        if (guizmoUsed)
        {
            glm::vec3 translation, rotation, scale;
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));

            tc.position = translation;
            tc.rotation = glm::radians(rotation);
            tc.scale = scale;
        }
    }

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

    bool opened = ImGui::TreeNodeEx((void*)entity, flags,"%s", tag.name.c_str());

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


static void DrawVec3Control(const char *label, glm::vec3 &values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label);

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text("%s", label);
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
}

template<typename T, typename UIFunction>
static void ImGuiDrawComponent(const char* name, Entity* entity, UIFunction uiFunction)
{
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    if (entity->HasComponent<T>())
    {
        auto& component = entity->GetComponent<T>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name);
        ImGui::PopStyleVar(
            );
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
        {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings"))
        {
            if (ImGui::MenuItem("Remove component"))
                removeComponent = true;

            ImGui::EndPopup();
        }

        if (open)
        {
            uiFunction(component);
            ImGui::TreePop();
        }

        if (removeComponent)
            entity->RemoveComponent<T>();
    }
}

void MainLayer::ImGuiDrawComponentsPanel()
{
    if(ImGui::Begin("Components"))
    {
        if(selectedEntity)
        {
            auto& name = selectedEntity->GetComponent<TagComponent>().name;
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            std::strncpy(buffer, name.c_str(), sizeof (buffer));
            if(ImGui::InputText("##Name", buffer, sizeof (buffer)))
            {
                name = std::string(buffer);
            }

            ImGui::SameLine();
            ImGui::PushItemWidth(-1);

            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("AddComponent");

            if (ImGui::BeginPopup("AddComponent"))
            {
                ImGui::EndPopup();
            }

            ImGui::PopItemWidth();

            ImGuiDrawComponent<TransformComponent>("Transform", selectedEntity, [](auto& component) {
                DrawVec3Control("Translation", component.position);
                glm::vec3 rotation = glm::degrees(component.rotation);
                DrawVec3Control("Rotation", rotation);
                component.rotation = glm::radians(rotation);
                DrawVec3Control("Scale", component.scale, 1.0f);
            });

        }
        ImGui::End();
    }
}

void MainLayer::ImGuiDrawContentBrowser()
{
    ImGui::Begin("Content Browser");

    float width = ImGui::GetContentRegionAvail().x;
    float padding = 8.0f;
    float thumbnailSize = 128.0f;
    float cellSize = padding + thumbnailSize;
    int columnCount = (int)(width / cellSize);
    if(columnCount < 1)
        columnCount = 1;

    if(contentBrowserPath != Application::GetCurrentLevel()->GetAssetsPath())
    {
        if(ImGui::Button("../"))
        {
            contentBrowserPath = contentBrowserPath.parent_path();
        }
    }
    ImGui::Columns(columnCount, 0, false);

    for(auto&& entry : std::filesystem::directory_iterator(contentBrowserPath))
    {
        const auto& path = entry.path();
        auto relativePath = std::filesystem::relative(path, contentBrowserPath);
        auto filenameString = relativePath.filename().string();

        if(entry.is_directory())
        {
            ImGui::ImageButton((ImTextureID)contentBrowserDirectoryIcon->GetId(), {cellSize, cellSize});
        }
        else
        {
            if(entry.path().filename().extension() == ".png" ||
                entry.path().filename().extension() == ".jpg" ||
                entry.path().filename().extension() == ".jpeg" ||
                entry.path().filename().extension() == ".bmp")
            {
                if(thumbnails.find(entry.path().string()) == thumbnails.end())
                {
                    thumbnails[entry.path().string()] = new Texture();
                    thumbnails[entry.path().string()]->InitFromFile(entry.path().c_str());
                }
                ImGui::ImageButton((ImTextureID)thumbnails[entry.path().string()]->GetId(), {cellSize, cellSize});
            }
            else
            {
                ImGui::ImageButton((ImTextureID)contentBrowserFileIcon->GetId(), {cellSize, cellSize});
            }
        }

        if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        {
            if(entry.is_directory())
            {
                contentBrowserPath /= path.filename();
            }
        }

        ImGui::TextWrapped("%s", filenameString.c_str());
        ImGui::NextColumn();
    }

    ImGui::Columns(1);
    ImGui::End();
}


