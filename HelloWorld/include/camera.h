#pragma once
#include "common_includes.h"

namespace dawn_engine {
enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };
const glm::vec3 DEFAULT_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const double DEFAULT_YAW = -90.0f;
const double DEFAULT_PITCH = 0.0f;
const double DEFAULT_SPEED = 2.5f;
const double DEFAULT_SENSITIVITY = 0.05f;
const double DEFAULT_FOV = 45.0f;
const glm::vec3 DEFAULT_WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
class Camera {
  private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 WORLD_UP{};
    double yaw;
    double pitch;
    double movementSpeed;
    double mouseSensitivity;
    double fov;
    void updateCameraFrame();

  public:
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), double yaw = DEFAULT_YAW, double pitch = DEFAULT_PITCH);
    glm::mat4 getViewMatrix();
    [[nodiscard]] GLfloat getFov() const; // indicate that the return value of a function shouldn't be ignored when you do a function call.
    [[nodiscard]] glm::vec3 getPos() const;
    void processKeyboard(CameraMovement, float);
    void processMouseMovement(double xOffset, double yOffset, GLboolean constrainPitch = true);
    void processMouseScroll(double yOffset);
};
} // namespace dawn_engine