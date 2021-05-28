#pragma once
#include "Types.h"
#include "Window.h"
#include "Layer.h"
#include <vector>
#include "Event.h"
#include "EditorCamera.h"
#include "UniformBufferObject.h"

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
        static Application* s_app;
        ApplicationConfiguration config;
        Window* window;
        std::vector<Layer*> layers;
        Renderer* renderer;
        std::vector<Event> events;
        std::vector<EventListener> event_listners;
        EditorCamera* editorCam;
        UniformBufferObject* cameraUBO;
    
    public:
        Application() = default;
        Application(const ApplicationConfiguration& config);
        ~Application();
        
        inline static Application* GetApp() { return s_app; }

        void Run();
        void AttachLayer(Layer* layer);
        void AddEvent(Event e);
        void AddEventListener(EventListener listener);
        
        inline ApplicationConfiguration* GetConfig() { return &config; }
        inline Window* GetWindow() { return window; }
        inline u32 GetLayerCount() { return layers.size(); }
        inline Renderer* GetRenderer() { return renderer; }
        inline EditorCamera* GetEditorCamera() { return editorCam; }
};