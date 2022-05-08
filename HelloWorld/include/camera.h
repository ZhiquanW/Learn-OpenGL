#pragma once
#include "common_includes.h"

namespace dawn_engine {
enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };
const glm::vec3 DEFAULT_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const float DEFAULT_YAW = -90.0f;
const float DEFAULT_PITCH = 0.0f;
const float DEFAULT_SPEED = 2.5f;
const float DEFAULT_SENSITIVITY = 0.05f;
const float DEFAULT_FOV = 45.0f;
const glm::vec3 DEFAULT_WORLDUP = glm::vec3(0.0f, 1.0f, 0.0f);
class Camera {
  private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 WORLDUP;
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float fov;
    void updateCameraFrame();

  public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH);
    glm::mat4 getViewMatrix();
    GLfloat getFov();
    glm::vec3 getPos();
    void processKeyboard(CameraMovement, float);
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
    void processMouseScroll(float yOffset);
};
} // namespace dawn_engine