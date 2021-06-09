#pragma once
#include "EditorCamera.h"
#include "Event.h"
#include "Layer.h"
#include "Types.h"
#include "UniformBufferObject.h"
#include "Window.h"
#include "Level.h"
#include "Framebuffer.h"
#include <vector>


class Renderer;

struct ApplicationConfiguration
{
    WindowConfiguration windowConfig;
    u8 opengl_version_major;
    u8 opengl_version_minor;
    b8 enable_imgui;
    b8 enable_imgui_docking;
};

class Application
{
  private:
    static Application* s_app;
    ApplicationConfiguration config;
    Window* window;
    std::vector<Layer*> layers;
    Renderer* renderer;
    std::vector<Event> events;
    std::vector<EventListener> event_listners;
    Level* currentLevel;
    Framebuffer* renderTarget;
    b8 blockEvents;
  public:
    Application(const ApplicationConfiguration &config);
    ~Application();

    void Run();
    void AttachLayer(Layer* layer);

    inline static Application *GetInstance()
    {
        return s_app;
    }

    inline static void AddEvent(Event e)
    {
        s_app->events.push_back(e);
    }

    inline static void AddEventListener(EventListener listener)
    {
        s_app->event_listners.push_back(listener);
    }

    inline static ApplicationConfiguration *GetConfig()
    {
        return &s_app->config;
    }

    inline static Window *GetWindow()
    {
        return s_app->window;
    }

    inline static u32 GetLayerCount()
    {
        return s_app->layers.size();
    }

    inline static Renderer *GetRenderer()
    {
        return s_app->renderer;
    }

    inline static void SetCurrentLevel(Level* level)
    {
        s_app->currentLevel = level;
    }

    inline static Level* GetCurrentLevel()
    {
        return s_app->currentLevel;
    }

    inline static Framebuffer* GetRenderTarget()
    {
        return s_app->renderTarget;
    }

    inline static void BlockEvents(b8 status)
    {
        s_app->blockEvents = status;
    }
};
