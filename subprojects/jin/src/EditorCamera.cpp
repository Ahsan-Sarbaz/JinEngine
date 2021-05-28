#include "EditorCamera.h"
#include <glm/gtc/matrix_transform.hpp>

EditorCamera::EditorCamera()
    :fov(70.0f), width(800.0f), height(600.0f), near(0.1f), far(3000.0f), eye({0.0f,0.0f,10.0f}), center({0.0f,0.0f,0.0f}), up({0.0f,1.0f,0.0f}), position({0.0f,0.0f,0.0f}), rotation({0.0f,0.0f,0.0f})
{
    proj = glm::perspectiveFov(fov, width, height, near, far);
    view = glm::lookAt(eye, center, up);
}

void EditorCamera::Update()
{
    proj = glm::perspectiveFov(fov, width, height, near, far);
    view = glm::lookAt(eye, center, up) * 
    glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), {1.0f, 0.0f, 0.0f}) * 
    glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), {0.0f, 1.0f, 0.0f}) *
    glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), {0.0f, 0.0f, 1.0f}) *
    glm::translate(glm::mat4(1.0f), position);
}