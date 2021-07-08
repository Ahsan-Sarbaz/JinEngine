#include "Application.h"

#include "Logger.h"
#include "Renderer.h"
#include "JinTime.h"
#include "Entity.h"
#include "Components.h"
#include "KeyCodes.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// TODO: NOTE: this is to be controlled at build time
#define ENABLE_IMGUI 1

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imguizmo.h"

Application* Application::s_app;

static void glfw_error_callback(int error, const char* description)
{
    LOG_ERROR("GLFW %d: %s\n", error, description);
}

static void glfw_window_framebuffer_resize(GLFWwindow* window, int width, int height)
{
    auto app = Application::GetInstance();
    app->GetConfig()->windowConfig.width = width;
    app->GetConfig()->windowConfig.height = height;

    Event e = {};
    e.type = EVENT_TYPE_WINDOW_RESIZE;
    e.data.width = width;
    e.data.height = height;
    app->AddEvent(e);
}

static void glfw_key_callback(GLFWwindow* window, int key ,int scancode, int action, int mod)
{
    auto app = Application::GetInstance();
    Event e = {};
    e.data.key_code = key;
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
    auto app =  Application::GetInstance();
    Event e = {};
    e.type = EVENT_TYPE_MOUSE_MOVE;
    e.data.mouse_x = x;
    e.data.mouse_y = y;
    app->AddEvent(e);
}

static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mod)
{
    auto app =  Application::GetInstance();
    Event e = {};
    e.data.mouse_button = button;
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

static void glfw_char_callback(GLFWwindow* window, unsigned int codepoint)
{
    auto app = Application::GetInstance();
    Event e = {};
    e.data.key_char = codepoint;
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
    glfwSetCharCallback(window->GetHandle(), glfw_char_callback);
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

    FramebufferConfiguration framebufferConfig = {};
    framebufferConfig.width = config.windowConfig.width;
    framebufferConfig.height= config.windowConfig.height;
    framebufferConfig.samples = 1;
    framebufferConfig.attachmentConfigs.push_back({FRAMEBUFFER_ATTACHMENT_FORMAT_RGBA8});
    framebufferConfig.attachmentConfigs.push_back({FRAMEBUFFER_ATTACHMENT_FORMAT_DEPTH});
    renderTarget = new Framebuffer(framebufferConfig);
    if(!renderTarget->IsComplete())
    {
        LOG_FATAL("Render Target not complete!\n");
        return;
    }

    EventListener renderTargetResizeEventListener = {};
    renderTargetResizeEventListener.object = renderTarget;
    renderTargetResizeEventListener.type = EVENT_TYPE_FRAME_BUFFER_RESIZE;
    renderTargetResizeEventListener.callback = [](void* object, Event e) {
        auto renderTarget = (Framebuffer*)object;
        renderTarget->Invalidate();
    };
}

Application::~Application()
{
    delete renderTarget;
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

    double current_time;
    double prev_time = glfwGetTime();
    double delta_time;
    
    auto time = GetTimeInternal();

    while (!glfwWindowShouldClose(window->GetHandle()))
    {
        glfwPollEvents();

        if(!blockEvents)
        {
            for (auto &&listner : event_listners)
            {
                for (auto &&event : events)
                {
                    if(listner.type & event.type)
                    {
                        listner.callback(listner.object, event);
                    }
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

        if(config.enable_imgui)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            if(config.enable_imgui_docking)
            {
                ImGui::DockSpaceOverViewport();
            }

            ImGuizmo::BeginFrame();
        }

        for (int i = layers.size() - 1; i >= 0; i--)
        {
            layers[i]->OnUpdate();
        }

        renderTarget->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(currentLevel != nullptr)
        {
            if(currentLevel->skybox != nullptr)
                renderer->DrawSkybox(currentLevel->skybox);

            for(auto& entity : currentLevel->registry.view<ModelComponent>())
            {
                auto materialComp = currentLevel->registry.try_get<MaterialComponent>(entity);
                auto& tranfrom = currentLevel->registry.get<TransformComponent>(entity);
                auto& modelComp = currentLevel->registry.get<ModelComponent>(entity);
                // TODO: move this to a transform thing
                glm::mat4 model_matrix = tranfrom.GetTransform();
                renderer->DrawModel(modelComp.model, model_matrix, materialComp ? materialComp->material : nullptr);
            }

            renderer->StartNewBatch();

            for(auto& entity : currentLevel->registry.view<SpriteComponent>())
            {
                auto& tranfrom = currentLevel->registry.get<TransformComponent>(entity);
                auto& spriteCompnonent = currentLevel->registry.get<SpriteComponent>(entity);

                renderer->DrawTexturedQuad(tranfrom.position, tranfrom.scale, spriteCompnonent.texture);
            }

            for(auto& entity : currentLevel->registry.view<SpriteSheetAnimationComponent>())
            {
                auto& tranfrom = currentLevel->registry.get<TransformComponent>(entity);
                auto& anim = currentLevel->registry.get<SpriteSheetAnimationComponent>(entity);

                renderer->DrawTexturedRectQuad(tranfrom.position, tranfrom.scale, anim.spriteSheetAnimation->GetSpriteSheet()->GetConfig().texture,
                anim.spriteSheetAnimation->GetSpriteSheet()->GetRect(anim.spriteSheetAnimation->GetLayout().current_frame));
                anim.spriteSheetAnimation->Update();
            }

            renderer->DrawCurrentBatch();
        }

        renderTarget->Unbind();

        if(config.enable_imgui)
        {
            if(ImGui::Begin("Debug"))
            {
                ImGui::Text("Render Target Size: %d x %d", renderTarget->GetConfig()->width, renderTarget->GetConfig()->height);
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
    s_app->layers.push_back(layer);
}
