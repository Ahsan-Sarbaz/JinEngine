#include "EditorCamera.h"
#include "JinTime.h"
#include "Application.h"

EditorCamera::EditorCamera()
    :position({0.0f, 0.0f,0.0f}), front({0.0f, 0.0f,-1.0f}), up({0.0f, 1.0f,0.0f}), worldUp(up), yaw(-90.0f), pitch(0.0f), movementSpeed(1.0f), mouseSensitivity(0.25f), zoom(40.0f)
{
    Update();
}

void EditorCamera::ProcessKeyboard(CameraMovment direction)
{
    float velocity = movementSpeed * GetDeltaTimeMs();
        
    if (direction == CAMERA_MOVEMENT_FORWARD)
    {
        position += front * velocity;
    }
    
    if (direction == CAMERA_MOVEMENT_BACKWARD)
    {
        position -= front * velocity;
    }
    
    if (direction == CAMERA_MOVEMENT_LEFT)
    {
        position -= right * velocity;
    }
    
    if (direction == CAMERA_MOVEMENT_RIGHT)
    {
        position += right * velocity;
    }
}

void EditorCamera::ProcessMouseMove(float xoffset, float yoffset, bool  constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    
    yaw   += xoffset;
    pitch += yoffset;
    
    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if ( constrainPitch )
    {
        if ( pitch > 89.0f )
        {
            pitch = 89.0f;
        }
        
        if ( pitch < -89.0f )
        {
            pitch = -89.0f;
        }
    }
    
    // Update Front, Right and Up Vectors using the updated Eular angles
    Update();
}

glm::mat4 EditorCamera::GetViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 EditorCamera::GetProjectionMatrix(float width, float height)
{
    return glm::perspective(70.0f, width / height, 0.1f, 3000.0f);
}

void EditorCamera::Update()
{
    glm::vec3 new_front;
    new_front.x = cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
    new_front.y = sin( glm::radians( pitch ) );
    new_front.z = sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
    front = glm::normalize( new_front );

    right = glm::normalize( glm::cross( front, worldUp ) );
    up = glm::normalize( glm::cross( right, front ) );
}
