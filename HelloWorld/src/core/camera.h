#pragma once

#include "common_includes.h"
#include "shader_uniform_variable.h"

namespace dawn_engine {
    enum CameraMovement {
        FORWARD, BACKWARD, LEFT, RIGHT
    };
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
        float yaw;
        float pitch;
        float moveSpeed = 2.5f;
        float mouseSensitivity = 0.05f;
        float fov = 45.0f;
        float zNear = 0.001f;
        float zFar = 100.0f;

        void updateCameraFrame();

    public:
        explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 9.0f), double yaw = DEFAULT_YAW, double pitch = DEFAULT_PITCH);

        glm::mat4 GetViewMatrix() const;

        glm::mat4 GetPerspectiveMatrix(float winWidth, float winHeight) const;

        [[nodiscard]] float GetZNear() const;

        float &getZNearMeta();

        [[nodiscard]] float GetZFar() const;

        float &getZFarMeta();

        float &getMoveSpeedMeta();

        float &getMouseSensitivityMeta();

        float &getFovMeta();

        glm::vec3 GetFront() const;

        glm::vec3 GetUp() const;

        glm::vec3 GetRight() const;

        std::vector<std::shared_ptr<ShaderUniformVariableBase *>> getUniforms() const;

        [[nodiscard]] GLfloat GetFov() const; // indicate that the return value of a function shouldn't be ignored when you do a function call.
        [[nodiscard]] glm::vec3 GetPosition() const;

        void processKeyboard(CameraMovement, float);

        void processMouseMovement(double xOffset, double yOffset, GLboolean constrainPitch = true);

        void processMouseScroll(double yOffset);
    };
} // namespace dawn_engine