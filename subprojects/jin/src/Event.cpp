#include "Event.h"
#include "Application.h"
#include <vector>

std::vector<EventListener> event_listners;
std::vector<Event> events;

void InitEventSystem(Application* app)
{
    app->events = &events;
    app->event_listners = &event_listners;
}

void RegisterListener(EventType type, PFNEVENTCALLBACK callback)
{
    event_listners.push_back({type, callback});
}


void PollEvents()
{
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
}
