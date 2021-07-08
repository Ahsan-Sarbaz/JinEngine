#pragma once
#include "Defines.h"
#include "Types.h"

class Input
{
public:
    static bool IsKeyDown(int key_code);
    static bool IsMouseButtonDown(int button);
    static double GetMousePosX();
    static double GetMousePosY();
};
