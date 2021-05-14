#pragma once
#include "Types.h"

enum EventType
{
    EVENT_TYPE_KEYBOARD_KEY_PRESS,
    EVENT_TYPE_KEYBOARD_KEY_DOWN,
    EVENT_TYPE_KEYBOARD_KEY_REPEAT,
    EVENT_TYPE_KEYBOARD_KEY_RELEASE,
    
    EVENT_TYPE_MOUSE_BUTTON_PRESS,
    EVENT_TYPE_MOUSE_BUTTON_DOWN,
    EVENT_TYPE_MOUSE_BUTTON_REPEAT,
    EVENT_TYPE_MOUSE_BUTTON_RELEASE,
    
    EVENT_TYPE_WINDOW_RESIZE,
    EVENT_TYPE_MOUSE_MOVE,
    
};

struct EventData
{
    union
    {
        u64 unsigned_long[4];
        u32 unsigned_int[8];
        u16 unsigned_short[16];
        u8 unsigned_char[32];
        
        double real_double[4];
        float real_float[8];
        i64 signed_long[4];
        i32 signed_int[8];
        i16 signed_short[16];
        i8 signed_char[32];
        struct
        {
            i32 key_char;
            i32 key_mods;
        };

        struct
        {
            double x;
            double y;
        };

        struct
        {
            double width;
            double height;
        };
        
    };
};

struct Event
{
    EventData data;
    EventType type;
};

typedef void(*PFNEVENTCALLBACK)(Event e);

struct EventListener
{
    EventType type;
    PFNEVENTCALLBACK callback;
};

struct Application;

void InitEventSystem(Application* app);
void RegisterListener(EventType type, PFNEVENTCALLBACK callback);
void PollEvents();