#pragma once
#include "Types.h"

enum EventType : u32
{
    EVENT_TYPE_KEYBOARD_KEY_PRESS = 0x01,
    EVENT_TYPE_KEYBOARD_KEY_DOWN = 0x02,
    EVENT_TYPE_KEYBOARD_KEY_REPEAT = 0x04,
    EVENT_TYPE_KEYBOARD_KEY_RELEASE = 0x08,
    
    EVENT_TYPE_MOUSE_BUTTON_PRESS = 0x10,
    EVENT_TYPE_MOUSE_BUTTON_DOWN = 0x20,
    EVENT_TYPE_MOUSE_BUTTON_REPEAT = 0x40,
    EVENT_TYPE_MOUSE_BUTTON_RELEASE = 0x80,
    
    EVENT_TYPE_WINDOW_RESIZE = 0x100,
    EVENT_TYPE_MOUSE_MOVE = 0x200,

    EVENT_TYPE_FRAME_BUFFER_RESIZE = 0x400
    
};

struct EventData
{
    union
    {
        struct
        {
            i32 key_code;
            u32 key_char;
            i32 key_mods;
        };

        struct
        {
            double mouse_x;
            double mouse_y;
            i32 mouse_button;
        };

        struct
        {
            i32 width;
            i32 height;
        };
    };
};

struct Event
{
    EventData data;
    u32 type;
};

typedef void(*PFNEVENTCALLBACK)(void* object, Event e);

struct EventListener
{
    u32 type;
    PFNEVENTCALLBACK callback;
    void* object;
};
