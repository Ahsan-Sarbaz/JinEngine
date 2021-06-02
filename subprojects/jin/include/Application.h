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


struct Renderer;

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
    static Application *s_app;
    ApplicationConfiguration config;
    Window *window;
    std::vector<Layer *> layers;
    Renderer *renderer;
    std::vector<Event> events;
    std::vector<EventListener> event_listners;
    EditorCamera *editorCam;
    UniformBufferObject *cameraUBO;
    Level* currentLevel;
    Framebuffer* renderTarget;

  public:
    Application() = default;
    Application(const ApplicationConfiguration &config);
    ~Application();

    inline static Application *GetInstance()
    {
        return s_app;
    }

    void Run();
    void AttachLayer(Layer *layer);
    void AddEvent(Event e);
    void AddEventListener(EventListener listener);

    inline ApplicationConfiguration *GetConfig()
    {
        return &config;
    }
    inline Window *GetWindow()
    {
        return window;
    }
    inline u32 GetLayerCount()
    {
        return layers.size();
    }
    inline Renderer *GetRenderer()
    {
        return renderer;
    }
    inline EditorCamera *GetEditorCamera()
    {
        return editorCam;
    }

    inline void SetCurrentLevel(Level* level)
    {
        currentLevel = level;
    }

    inline Level* GetCurrentLevel()
    {
        return currentLevel;
    }

    inline Framebuffer* GetRenderTarget()
    {
        return renderTarget;
    }
};
