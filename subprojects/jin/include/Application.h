#pragma once
#include "Types.h"
#include "Window.h"
#include "Layer.h"
#include <vector>
#include "Event.h"


struct Renderer;

struct ApplicationConfiguration
{
    WindowConfiguration windowConfig;
    u8 opengl_version_major;
    u8 opengl_version_minor;
    b8 enable_imgui;
};

class Application
{
    private:
        ApplicationConfiguration config;
        Window* window;
        Layer* layers[32];
        u32 layersCount;
        Renderer* renderer;
        std::vector<Event> events;
        std::vector<EventListener> event_listners;

    public:
        Application() = default;
        ~Application();
        void Init(const ApplicationConfiguration& config);
        void Run();
        void AttachLayer(Layer* layer);
        void AddEvent(Event e);
        void AddEventListener(EventListener listener);
        
        inline ApplicationConfiguration* GetConfig() { return &config; }
        inline Window* GetWindow() { return window; }
        inline u32 GetLayerCount() { return layersCount; }
        inline Renderer* GetRenderer() { return renderer; }
};