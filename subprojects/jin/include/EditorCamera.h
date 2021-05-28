#pragma once
#include <glm/glm.hpp>

class EditorCamera
{
private:
    glm::mat4 view;
    glm::mat4 proj;
    float width, height, fov, near, far;
    glm::vec3 eye, center, up;
    glm::vec3 position;
    glm::vec3 rotation;
public:
    EditorCamera();
    void Update();
    
    inline glm::mat4 GetView() { return view; }
    inline glm::mat4 GetProjection() { return proj; }

    inline void SetEye(const glm::vec3& e) { eye = e; }
    inline void SetCenter(const glm::vec3& c) { center = c; }
    inline void SetUp(const glm::vec3& u) { up = u; }
    inline void Move(const glm::vec3& by) { position += by; }
    inline void Rotate(const glm::vec3& by) { rotation+= by; }
    inline glm::vec3* GetPosition() { return &position; }
    inline glm::vec3* GetRotation() { return &rotation; }
};