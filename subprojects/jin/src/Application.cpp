#include "Application.h"

#include "Logger.h"
#include "Renderer.h"
#include "Time.h"
#include "Entity.h"
#include "Components.h"
#include "Maths.h"
#include "KeyCodes.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

// TODO: NOTE: this is to be controlled at build time
#define ENABLE_IMGUI 1

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Application* Application::s_app;

static void glfw_error_callback(int error, const char* description)
{
    LOG_ERROR("GLFW %d: %s\n", error, description);
}

static void glfw_window_framebuffer_resize(GLFWwindow* window, int width, int height)
{
    auto app =  (Application*)glfwGetWindowUserPointer(window);
    app->GetConfig()->windowConfig.width = width;
    app->GetConfig()->windowConfig.height = height;

    Event e = {};
    e.type = EVENT_TYPE_WINDOW_RESIZE;
    e.data.signed_int[0] = width;
    e.data.signed_int[1] = height;
    app->AddEvent(e);
}

static void glfw_key_callback(GLFWwindow* window, int key ,int scancode, int action, int mod)
{
    auto app =  (Application*)glfwGetWindowUserPointer(window);
    Event e = {};
    e.data.key_char = key;
    e.data.key_mods = mod;
    if (action == GLFW_PRESS)
    {
        e.type = EVENT_TYPE_KEYBOARD_KEY_DOWN;
    }
    else if(action == GLFW_REPEAT)
    {
        e.type = EVENT_TYPE_KEYBOARD_KEY_REPEAT;
    }
    else if(action == GLFW_RELEASE)
    {
        e.type = EVENT_TYPE_KEYBOARD_KEY_RELEASE;
    }    

    app->AddEvent(e);
}

static void glfw_cursor_pos_callback(GLFWwindow* window, double x, double y)
{
    auto app =  (Application*)glfwGetWindowUserPointer(window);
    Event e = {};
    e.type = EVENT_TYPE_MOUSE_MOVE;
    e.data.real_double[0] = x;
    e.data.real_double[1] = y;
    app->AddEvent(e);
}

static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mod)
{
    auto app =  (Application*)glfwGetWindowUserPointer(window);
    Event e = {};
    e.data.signed_int[0] = button;
    if (action == GLFW_PRESS)
    {
        e.type = EVENT_TYPE_MOUSE_BUTTON_DOWN;
    }
    else if(action == GLFW_REPEAT)
    {
        e.type = EVENT_TYPE_MOUSE_BUTTON_REPEAT;
    }
    else if(action == GLFW_RELEASE)
    {
        e.type = EVENT_TYPE_MOUSE_BUTTON_RELEASE;
    }    

    app->AddEvent(e);
}

Application::Application(const ApplicationConfiguration& _config)
{
    s_app = this;
    config = _config;
    auto time = GetTimeInternal();
    *time = new Time;
    (*time)->time_scale = 1;

    glfwSetErrorCallback(glfw_error_callback);

    if(glfwInit() != GLFW_TRUE)
    {
        LOG_FATAL("GLFW INIT Failed Cannot Continue!\n");
    }

    LOG_INFO("GLFW INIT SUCCESS!\n");
    if(_config.opengl_version_major == 0)
    {
        config.opengl_version_major = 3;
        config.opengl_version_minor = 3;
    }

    config.windowConfig.gl_contex_version_major = config.opengl_version_major;
    config.windowConfig.gl_contex_version_minor = config.opengl_version_minor;
    
    window = new Window();
    window->Init(config.windowConfig);

    if(this->window == nullptr)
    {
        LOG_FATAL("Failed to Create Window Cannot Continue\n");
    }
    
    glfwSetWindowUserPointer(window->GetHandle(), this);
    glfwSetFramebufferSizeCallback(window->GetHandle(), glfw_window_framebuffer_resize);
    glfwSetKeyCallback(window->GetHandle(), glfw_key_callback);
    glfwSetCursorPosCallback(window->GetHandle(), glfw_cursor_pos_callback);
    glfwSetMouseButtonCallback(window->GetHandle(), glfw_mouse_button_callback);

    /// update the window size if the window creation changed it
    this->config.windowConfig.width = window->GetConfig()->width;
    this->config.windowConfig.height = window->GetConfig()->height;

    LOG_INFO("Created Window Successfuly %dx%d : %s\n", config.windowConfig.width, config.windowConfig.height, config.windowConfig.title);
    if(glewInit() != GLEW_OK)
    {
        LOG_FATAL("GLEW INIT Failed Cannot Continue!\n");
    }

    LOG_INFO("GLEW INIT SUCCESS! USING RENDERER: %s\n", glGetString(GL_RENDERER));
    LOG_INFO("OpenGL Version: %s\n", glGetString(GL_VERSION));

    config.enable_imgui = config.enable_imgui ? ENABLE_IMGUI : 0;

    if(config.enable_imgui)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window->GetHandle(), true);
        const char* glsl_version = "#version 130";
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    RendererConfiguration rendererConfig = {};
    rendererConfig.app = this;
    rendererConfig.renderer_resolution_x = config.windowConfig.width;
    rendererConfig.renderer_resolution_y = config.windowConfig.height;
    rendererConfig.enable_batch_renderering = TRUE;
    rendererConfig.batch_renderer_max_quads = 100;
    renderer = new Renderer(rendererConfig);

    editorCam = new EditorCamera();
    // editorCam->SetEye(glm::vec3{0.f,0.f,1000.f});
    EventListener editorCameraKeyboardEventListener = {};
    editorCameraKeyboardEventListener .type = EVENT_TYPE_KEYBOARD_KEY_REPEAT;
    editorCameraKeyboardEventListener .callback = [](Event e){
        switch(e.data.key_char)
        {
            case JIN_KEY_W:
            case JIN_KEY_UP:
                Application::GetApp()->GetEditorCamera()->ProcessKeyboard(CAMERA_MOVEMENT_FORWARD);
            break;
            case JIN_KEY_S:
            case JIN_KEY_DOWN:
                Application::GetApp()->GetEditorCamera()->ProcessKeyboard(CAMERA_MOVEMENT_BACKWARD);
            break;
            case JIN_KEY_A:
            case JIN_KEY_LEFT:
                Application::GetApp()->GetEditorCamera()->ProcessKeyboard(CAMERA_MOVEMENT_LEFT);
            break;
            case JIN_KEY_D:
            case JIN_KEY_RIGHT:
                Application::GetApp()->GetEditorCamera()->ProcessKeyboard(CAMERA_MOVEMENT_RIGHT);
            break;
        }
    };
    AddEventListener(editorCameraKeyboardEventListener);
    EventListener editorCameraMouseEventListener = {};
    editorCameraMouseEventListener.type = EVENT_TYPE_MOUSE_MOVE;
    editorCameraMouseEventListener.callback = [](Event e)
    {
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
        Application::GetApp()->GetEditorCamera()->ProcessMouseMove(xOffset, yOffset);
    };
    AddEventListener(editorCameraMouseEventListener);

    cameraUBO = new UniformBufferObject(UNIFORM_BUFFER_OBJECT_TYPE_DYNAMIC_DRAW);
    cameraUBO->SetBindingIndex(0);
    cameraUBO->SetData(sizeof(glm::mat4) * 2, nullptr);
}

Application::~Application()
{

    delete editorCam;
    delete renderer;
    glfwDestroyWindow(window->GetHandle());
    delete window;
    glfwTerminate();
}

void  Application::Run()
{
    glClearColor(0, 0, 0, 1);

    for (int i = layers.size() - 1; i >= 0; i--)
    {
        layers[i]->OnStart();
    }

    double current_time = glfwGetTime();
    double prev_time = glfwGetTime();
    double delta_time = current_time - prev_time;
    
    auto time = GetTimeInternal();

    auto registry = GetEnTTRegistry();

    while (!glfwWindowShouldClose(window->GetHandle()))
    {
        glfwPollEvents();
    
        for (auto &&listner : event_listners)
        {
            for (auto &&event : events)
            {
                if(listner.type == event.type)
                {
                    listner.callback(event);
                }
            }
        }    

        events.clear();

        current_time = glfwGetTime();
        delta_time = (current_time - prev_time);
        prev_time = current_time;
        (*time)->time_since_start += delta_time;
        (*time)->delta_time = (delta_time * (*time)->time_scale) * 1000.0;
        (*time)->delta_time_ms = (delta_time * (*time)->time_scale);

        
        if(glfwGetKey(window->GetHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window->GetHandle(), 1);
        }

        glViewport(0, 0, config.windowConfig.width, config.windowConfig.height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        if(config.enable_imgui)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        glm::mat4 cameraUBOData[2] = {
            editorCam->GetProjectionMatrix((float)config.windowConfig.width, (float)config.windowConfig.height), editorCam->GetViewMatrix()
        };
        
        cameraUBO->SetSubData(0, sizeof(cameraUBOData), cameraUBOData);
        
        
        for (int i = layers.size() - 1; i >= 0; i--)
        {
            layers[i]->OnUpdate();
        }

        renderer->StartNewBatch();
        for(auto& entity : registry.view<SpriteComponent>())
        {
            auto& tranfrom = registry.get<TransformComponent>(entity);
            auto& spriteCompnonent = registry.get<SpriteComponent>(entity);

            renderer->DrawTexturedQuad(ToVec2(tranfrom.position), ToVec2(tranfrom.size), spriteCompnonent.texture);
        }

        for(auto& entity : registry.view<SpriteSheetAnimationComponent>())
        {
            auto& tranfrom = registry.get<TransformComponent>(entity);
            auto& anim = registry.get<SpriteSheetAnimationComponent>(entity);

            renderer->DrawTexturedRectQuad(ToVec2(tranfrom.position), ToVec2(tranfrom.size), anim.spriteSheetAnimation->GetSpriteSheet()->GetConfig().texture, 
            anim.spriteSheetAnimation->GetSpriteSheet()->GetRect(anim.spriteSheetAnimation->GetLayout().current_frame));
            anim.spriteSheetAnimation->Update();
        }

        renderer->DrawCurrentBatch();

        if(config.enable_imgui)
        {
            if(ImGui::Begin("Debug"))
            {

                // ImGui::InputFloat3("Camera Position", (float*)editorCam->GetPosition());
                // ImGui::InputFloat3("Camera Rotation", (float*)editorCam->GetRotation());

                ImGui::Text("Attached Layers");
                for (int i = layers.size() - 1; i >= 0; i--)
                {
                    ImGui::Text("Layer: %s", layers[i]->GetName());
                }

                if(ImGui::TreeNode("Batch Renderer Stats"))
                {
                    ImGui::Text("Max Quads Count: %d", renderer->GetBatchStats()->max_quad_count);
                    ImGui::Text("Max Verts Count: %d", renderer->GetBatchStats()->max_vertex_count);
                    ImGui::Text("Max Index Count: %d", renderer->GetBatchStats()->max_index_count);
                    ImGui::Text("Draw Count P/F : %d", renderer->GetBatchStats()->draw_count);
                    ImGui::Text("Quad Count P/F : %d", renderer->GetBatchStats()->quad_count);
                    ImGui::Text("Verts Count P/F: %d", renderer->GetBatchStats()->vertex_count);
                    ImGui::Text("Index Count P/F: %d", renderer->GetBatchStats()->index_count);
                    ImGui::TreePop();
                }

                renderer->ResetRendererStats();
                ImGui::Text("Enitites");
                for(auto& entity : registry.view<TagComponent>())
                {
                    ImGui::Text("Name: %s",  registry.get<TagComponent>(entity).name);

                    TransformComponent* transform;
                    if((transform = registry.try_get<TransformComponent>(entity)) != nullptr)
                    {
                        ImGui::Text("Position: %.2f, %.2f, %.2f", transform->position.x, transform->position.y, transform->position.z);
                        ImGui::Text("Rotation: %.2f, %.2f, %.2f", transform->rotation.x, transform->rotation.y, transform->rotation.z);
                        ImGui::Text("Size:     %.2f, %.2f, %.2f", transform->size.x, transform->size.y, transform->size.z);
                    }

                    ImGui::Separator();

                    SpriteSheetAnimationComponent* spriteSheetAnimComp;
                    if((spriteSheetAnimComp = registry.try_get<SpriteSheetAnimationComponent>(entity)) != nullptr)
                    {

                        ImGui::Text("Anim Name: %s", spriteSheetAnimComp->spriteSheetAnimation->GetLayout().name);
                        ImGui::Text("Anim Start: %d", spriteSheetAnimComp->spriteSheetAnimation->GetLayout().start_frame);
                        ImGui::Text("Anim End: %d", spriteSheetAnimComp->spriteSheetAnimation->GetLayout().end_frame);
                        ImGui::Text("Anim FPS: %f", spriteSheetAnimComp->spriteSheetAnimation->GetLayout().frames_per_second);
                        ImGui::Text("Anim Total Time: %f", spriteSheetAnimComp->spriteSheetAnimation->GetLayout().total_time);
                        ImGui::Text("Anim Time Accum: %f", spriteSheetAnimComp->spriteSheetAnimation->GetLayout().time_accumulator);
                        ImGui::Text("Anim Time Step:  %f", spriteSheetAnimComp->spriteSheetAnimation->GetLayout().time_step);
                        ImGui::Text("Anim Current Frame: %d", spriteSheetAnimComp->spriteSheetAnimation->GetLayout().current_frame);
                        ImGui::Separator();                    
                    }
                }
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        
        glfwSwapBuffers(window->GetHandle());
    }

    for (int i = layers.size() - 1; i >= 0; i--)
    {
        layers[i]->OnEnd();
    }
    
    if(config.enable_imgui)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}

void Application::AttachLayer(Layer* layer)
{
    if(layer == nullptr)
    {
        LOG_ERROR("You are trying to attach a null Layer!\n");
        return;
    }
    layers.push_back(layer);
}

void Application::AddEvent(Event e)
{
    events.push_back(e);
}

void Application::AddEventListener(EventListener listener)
{
    event_listners.push_back(listener);
}
