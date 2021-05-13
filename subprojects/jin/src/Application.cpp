#include "Application.h"

#include "Memory.h"
#include "Logger.h"
#include "Renderer.h"
#include "Time.h"
#include "Entity.h"
#include "Components.h"
#include "Maths.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// TODO: NOTE: this is to be controlled at build time
#define ENABLE_IMGUI 1

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

static void glfw_error_callback(int error, const char* description)
{
    LOG_ERROR("GLFW %d: %s\n", error, description);
}

static void glfw_window_framebuffer_resize(GLFWwindow* window, int width, int height)
{
    auto app =  (Application*)glfwGetWindowUserPointer(window);
    app->config.windowConfig.width = width;
    app->config.windowConfig.height = height;

    Event e = {};
    e.type = EVENT_TYPE_WINDOW_RESIZE;
    e.data.signed_int[0] = width;
    e.data.signed_int[1] = height;
    app->events->push_back(e);
}

static void glfw_key_callback(GLFWwindow* window, int key ,int scancode, int action, int mod)
{
    auto app =  (Application*)glfwGetWindowUserPointer(window);
    Event e = {};
    e.data.signed_int[0] = key;
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
    
    app->events->emplace_back(e);
}

static void glfw_cursor_pos_callback(GLFWwindow* window, double x, double y)
{
    auto app =  (Application*)glfwGetWindowUserPointer(window);
    Event e = {};
    e.type = EVENT_TYPE_MOUSE_MOVE;
    e.data.real_double[0] = x;
    e.data.real_double[1] = y;
    app->events->push_back(e);
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
    
    app->events->emplace_back(e);
}

Application* CreateApplication(const ApplicationConfiguration& config)
{
    Application* app = (Application*)MemAlloc(sizeof(Application), MEMORY_TAG_STRUCT);
    app->config = config;

    auto time = GetTimeInternal();
    *time = new Time;
    (*time)->time_scale = 1;

    glfwSetErrorCallback(glfw_error_callback);

    if(glfwInit() != GLFW_TRUE)
    {
        LOG_FATAL("GLFW INIT Failed Cannot Continue!\n");
        return nullptr;
    }

    LOG_INFO("GLFW INIT SUCCESS!\n");
    if(config.opengl_version_major == 0)
    {
        app->config.opengl_version_major = 3;
        app->config.opengl_version_minor = 3;
    }

    app->config.windowConfig.gl_contex_version_major = app->config.opengl_version_major;
    app->config.windowConfig.gl_contex_version_minor = app->config.opengl_version_minor;
    
    app->window = CreateWindow(app->config.windowConfig);

    if(app->window == nullptr)
    {
        LOG_FATAL("Failed to Create Window Cannot Continue\n");
        return nullptr;
    }
    
    glfwSetWindowUserPointer(app->window->handle, app);
    glfwSetFramebufferSizeCallback(app->window->handle, glfw_window_framebuffer_resize);
    glfwSetKeyCallback(app->window->handle, glfw_key_callback);
    glfwSetCursorPosCallback(app->window->handle, glfw_cursor_pos_callback);
    glfwSetMouseButtonCallback(app->window->handle, glfw_mouse_button_callback);

    /// update the window size if the window creation changed it
    app->config.windowConfig.width = app->window->config.width;
    app->config.windowConfig.height = app->window->config.height;

    LOG_INFO("Created Window Successfuly %dx%d : %s\n", app->config.windowConfig.width, app->config.windowConfig.height, app->config.windowConfig.title);
    if(glewInit() != GLEW_OK)
    {
        LOG_FATAL("GLEW INIT Failed Cannot Continue!\n");
        return nullptr;
    }

    LOG_INFO("GLEW INIT SUCCESS! USING RENDERER: %s\n", glGetString(GL_RENDERER));
    LOG_INFO("OpenGL Version: %s\n", glGetString(GL_VERSION));

    app->config.enable_imgui = app->config.enable_imgui ? ENABLE_IMGUI : 0;

    if(app->config.enable_imgui)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(app->window->handle, true);
        const char* glsl_version = "#version 130";
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    RendererConfiguration rendererConfig = {};
    rendererConfig.app = app;
    rendererConfig.renderer_resolution_x = config.windowConfig.width;
    rendererConfig.renderer_resolution_y = config.windowConfig.height;
    rendererConfig.enable_batch_renderering = TRUE;
    rendererConfig.batch_renderer_max_quads = 100;
    app->renderer = CreateRenderer(rendererConfig);

    InitEventSystem(app);
    return app;
}

void DeleteApplication(Application* app)
{
    for(u32 i = 0; i < app->layersCount; ++i)
    {
        LOG_INFO("Deleting Layer: %s\n", app->layers[i]->config.name);
        DeleteLayer(app->layers[i]);
    }

    DeleteRenderer(app->renderer);
    DeleteWindow(app->window);
    glfwTerminate();
    MemFree(app, sizeof(Application), MEMORY_TAG_STRUCT);
    LOG_INFO("Application delete Success!\n");
}

void RunApplication(Application* app)
{
    glClearColor(0, 0, 0, 1);

    for(int i = app->layersCount; i > 0; --i)
    {
        LOG_INFO("Starting Layer: %s\n", app->layers[i - 1]->config.name);
        
        app->layers[i - 1]->config.onStart(app);
    }

    double current_time = glfwGetTime();
    double prev_time = glfwGetTime();
    double delta_time = current_time - prev_time;
    
    auto time = GetTimeInternal();

    auto registry = GetEnTTRegistry();

    while (!glfwWindowShouldClose(app->window->handle))
    {
        glfwPollEvents();
        PollEvents();

        current_time = glfwGetTime();
        delta_time = (current_time - prev_time);
        prev_time = current_time;
        (*time)->time_since_start += delta_time;
        (*time)->delta_time = (delta_time * (*time)->time_scale) * 1000.0;
        (*time)->delta_time_ms = (delta_time * (*time)->time_scale);

        
        if(glfwGetKey(app->window->handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(app->window->handle, 1);
        }

        glViewport(0, 0, app->config.windowConfig.width, app->config.windowConfig.height);
        glClear(GL_COLOR_BUFFER_BIT);
    
        if(app->config.enable_imgui)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        StartNewBatch(app->renderer);

        for(int i = app->layersCount; i > 0; --i)
        {
            app->layers[i - 1]->config.onUpdate(app);
        }

        for(auto& entity : registry.view<SpriteComponent>())
        {
            auto& tranfrom = registry.get<TransformComponent>(entity);
            auto& spriteCompnonent = registry.get<SpriteComponent>(entity);

            DrawTexturedQuad(ToVec2(tranfrom.position), ToVec2(tranfrom.size), spriteCompnonent.texture);
        }

        for(auto& entity : registry.view<SpriteSheetAnimationComponent>())
        {
            auto& tranfrom = registry.get<TransformComponent>(entity);
            auto& anim = registry.get<SpriteSheetAnimationComponent>(entity);

            DrawTexturedRectQuad(ToVec2(tranfrom.position), ToVec2(tranfrom.size), anim.spriteSheetAnimation->sprite_sheet->config.texture, 
            anim.spriteSheetAnimation->sprite_sheet->rects[anim.spriteSheetAnimation->layout.current_frame]);
            UpdateSpriteSheetAnimation(anim.spriteSheetAnimation);
        }

        DrawCurrentBatch(app->renderer);

        if(app->config.enable_imgui)
        {
            if(ImGui::Begin("Debug"))
            {
                ImGui::Text("Attached Layers");
                for(int i = app->layersCount; i > 0; --i)
                {
                    ImGui::Text("Layer: %s", app->layers[i - 1]->config.name);
                }
                auto memory_state = GetMemoryState();
                ImGui::Separator();
                if(ImGui::TreeNode("Memory"))
                {
                    ImGui::Text("Total : %d B, %.2f KB, %.2f MB", 
                    memory_state->total_memory, (float)memory_state->total_memory / 1024, (float)memory_state->total_memory / (1024 * 1024));
                    ImGui::Text("Overall : %d B, %.2f KB, %.2f MB", 
                    memory_state->memory_consumption, (float)memory_state->memory_consumption/ 1024, (float)memory_state->memory_consumption / (1024 * 1024));    
                    for (i32 i = 0; i < MEMORY_TAG_COUNT; i++)
                    {
                        ImGui::Text("\t%s : %d B, %.2f KB, %.2f MB",
                        MemoryTagToString((MEMORY_TAG)i), memory_state->memory_tag[i],
                        (float)memory_state->memory_tag[i] / 1024, 
                        (float)memory_state->memory_tag[i] / (1024 * 1024));
                    }

                    ImGui::TreePop();
                }

                if(ImGui::TreeNode("Batch Renderer Stats"))
                {
                    ImGui::Text("Max Quads Count: %d", app->renderer->batchStats->max_quad_count);
                    ImGui::Text("Max Verts Count: %d", app->renderer->batchStats->max_vertex_count);
                    ImGui::Text("Max Index Count: %d", app->renderer->batchStats->max_index_count);
                    ImGui::Text("Draw Count P/F : %d", app->renderer->batchStats->draw_count);
                    ImGui::Text("Quad Count P/F : %d", app->renderer->batchStats->quad_count);
                    ImGui::Text("Verts Count P/F: %d", app->renderer->batchStats->vertex_count);
                    ImGui::Text("Index Count P/F: %d", app->renderer->batchStats->index_count);
                    ImGui::TreePop();
                }

                ResetRendererStats(app->renderer);
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

                        ImGui::Text("Anim Name: %s", spriteSheetAnimComp->spriteSheetAnimation->layout.name);
                        ImGui::Text("Anim Start: %d", spriteSheetAnimComp->spriteSheetAnimation->layout.start_frame);
                        ImGui::Text("Anim End: %d", spriteSheetAnimComp->spriteSheetAnimation->layout.end_frame);
                        ImGui::Text("Anim FPS: %f", spriteSheetAnimComp->spriteSheetAnimation->layout.frames_per_second);
                        ImGui::Text("Anim Total Time: %f", spriteSheetAnimComp->spriteSheetAnimation->layout.total_time);
                        ImGui::Text("Anim Time Accum: %f", spriteSheetAnimComp->spriteSheetAnimation->layout.time_accumulator);
                        ImGui::Text("Anim Time Step:  %f", spriteSheetAnimComp->spriteSheetAnimation->layout.time_step);
                        ImGui::Text("Anim Current Frame: %d", spriteSheetAnimComp->spriteSheetAnimation->layout.current_frame);
                        ImGui::Separator();                    
                    }
                }
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        
        glfwSwapBuffers(app->window->handle);
    }

    for(int i = app->layersCount; i > 0; --i)
    {
        LOG_INFO("Ending Layer: %s\n", app->layers[i - 1]->config.name);
        app->layers[i - 1]->config.onEnd(app);
    }

    if(app->config.enable_imgui)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}

void ApplicationAttachLayer(Application* app, Layer* layer)
{
    if(layer == nullptr)
    {
        LOG_ERROR("You are trying to attach a null Layer!\n");
        return;
    }
    app->layers[app->layersCount] = layer;
    app->layersCount++;
}
