#include "Application.h"

#include "Memory.h"
#include "Logger.h"
#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define ENABLE_IMGUI 1

#if ENABLE_IMGUI

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#endif

static void glfw_error_callback(int error, const char* description)
{
    LOG_ERROR("GLFW %d: %s\n", error, description);
}

static void glfw_window_framebuffer_resize(GLFWwindow* window, int width, int height)
{
    auto app =  (Application*)glfwGetWindowUserPointer(window);
    app->config.windowConfig.width = width;
    app->config.windowConfig.height = height;
}

Application* CreateApplication(const ApplicationConfiguration& config)
{
    Application* app = (Application*)MemAlloc(sizeof(Application), MEMORY_TAG_STRUCT);
    app->config = config;
    
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

#if ENABLE_IMGUI

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(app->window->handle, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
#endif

    RendererConfiguration rendererConfig = {};
    rendererConfig.app = app;
    rendererConfig.renderer_resolution_x = config.windowConfig.width;
    rendererConfig.renderer_resolution_y = config.windowConfig.height;
    rendererConfig.enable_batch_renderering = TRUE;
    rendererConfig.batch_renderer_max_quads = 100;
    app->renderer = CreateRenderer(rendererConfig);

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

#if ENABLE_IMGUI
    ImGuiIO& io = ImGui::GetIO(); (void)io;
#endif

    for(int i = app->layersCount; i > 0; --i)
    {
        LOG_INFO("Starting Layer: %s\n", app->layers[i - 1]->config.name);
        
        app->layers[i - 1]->config.onStart(app);
    }

    while (!glfwWindowShouldClose(app->window->handle))
    {
        glfwPollEvents();
        
        if(glfwGetKey(app->window->handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(app->window->handle, 1);
        }

        glViewport(0, 0, app->config.windowConfig.width, app->config.windowConfig.height);
        glClear(GL_COLOR_BUFFER_BIT);
    
#if ENABLE_IMGUI

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
#endif

        for(int i = app->layersCount; i > 0; --i)
        {
            app->layers[i - 1]->config.onUpdate(app);
        }

#if ENABLE_IMGUI

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
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

        glfwSwapBuffers(app->window->handle);
    }

    for(int i = app->layersCount; i > 0; --i)
    {
        LOG_INFO("Ending Layer: %s\n", app->layers[i - 1]->config.name);
        app->layers[i - 1]->config.onEnd(app);
    }

#if ENABLE_IMGUI

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif
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
