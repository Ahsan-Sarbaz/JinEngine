#include "Input.h"
#include "Application.h"
#include <GLFW/glfw3.h>
#include "KeyCodes.h"

bool Input::IsKeyDown(int key_code)
{
    return glfwGetKey(Application::GetInstance()->GetWindow()->GetHandle(), key_code) == GLFW_PRESS;
}

bool Input::IsMouseButtonDown(int button)
{
    return glfwGetMouseButton(Application::GetInstance()->GetWindow()->GetHandle(), button) == GLFW_PRESS;
}

double Input::GetMousePosX()
{
    double x, y;
    glfwGetCursorPos(Application::GetInstance()->GetWindow()->GetHandle(), &x, &y);
    return x;
}

double Input::GetMousePosY()
{
    double x, y;
    glfwGetCursorPos(Application::GetInstance()->GetWindow()->GetHandle(), &x, &y);
    return y;
}
