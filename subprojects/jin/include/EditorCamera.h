#pragma once
#include <glm/glm.hpp>

enum CameraMovment
{
    CAMERA_MOVEMENT_FORWARD,
    CAMERA_MOVEMENT_BACKWARD,
    CAMERA_MOVEMENT_LEFT,
    CAMERA_MOVEMENT_RIGHT,
};

class EditorCamera
{
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    float yaw;
    float pitch;
    
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

public:
    EditorCamera();
    void ProcessKeyboard(CameraMovment direction);
    void ProcessMouseMove(float xoffset, float yoffset, bool  constrainPitch = true);
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix(float width, float height);
    inline glm::vec3 GetPosition() { return position; }
    inline glm::vec3 GetFront() { return front; }
    inline float GetZoom() { return zoom; }


private:
    void Update();
};