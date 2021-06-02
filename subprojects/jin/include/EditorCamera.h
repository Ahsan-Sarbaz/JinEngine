#pragma once
#include "Types.h"

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
    inline float GetMovementSpeed() { return movementSpeed; }
    inline float GetMouseSensitivity() { return mouseSensitivity; }

    inline void SetPosition(const glm::vec3& pos) { position = pos; }
    inline void SetZoom(float _zoom) { zoom = _zoom; }
    inline void SetMovementSpeed(float speed) { movementSpeed = speed; }
    inline void SetMouseSensitivity(float sensitivity) { mouseSensitivity = sensitivity; }

  private:
    void Update();
};
