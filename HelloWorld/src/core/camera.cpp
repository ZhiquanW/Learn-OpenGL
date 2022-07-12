#include "camera.h"

namespace dawn_engine {

    Camera::Camera(glm::vec3 position, double yaw, double pitch) : position(position),
                                                                   front(DEFAULT_FRONT),
                                                                   moveSpeed(DEFAULT_SPEED),
                                                                   fov(DEFAULT_FOV), yaw(yaw),
                                                                   pitch(pitch),
                                                                   mouseSensitivity(
                                                                           DEFAULT_SENSITIVITY) {
        this->WORLD_UP = DEFAULT_WORLD_UP;
        this->updateCameraFrame();
    }

    void Camera::updateCameraFrame() {
        this->front.x = (float) cos(glm::radians(this->yaw) * cos(glm::radians(this->pitch)));
        this->front.y = (float) sin(glm::radians(this->pitch));
        this->front.z = (float) sin(glm::radians(this->yaw) * cos(glm::radians(pitch)));
        this->front = glm::normalize(this->front);
        this->right = glm::normalize(glm::cross(this->front, this->WORLD_UP));
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }

    glm::mat4 Camera::GetViewMatrix() const {
        return glm::lookAt(this->position, this->position + this->front, this->up);
    }

    GLfloat Camera::GetFov() const { return this->fov; }

    glm::vec3 Camera::GetPosition() const { return this->position; }

    void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
        float distance = (float) this->moveSpeed * deltaTime;

        if (direction == FORWARD) {
            this->position += this->front * distance;
        } else if (direction == BACKWARD) {
            this->position -= this->front * distance;
        } else if (direction == LEFT) {
            this->position -= this->right * distance;
        } else if (direction == RIGHT) {
            this->position += this->right * distance;
        }
    }
// processes input received from a mouse input system. Expects the offset value
// in both the x and y direction.

    void
    Camera::processMouseMovement(GLdouble xOffset, GLdouble yOffset, GLboolean constrainPitch) {
        xOffset *= this->mouseSensitivity;
        yOffset *= this->mouseSensitivity;

        this->yaw += xOffset;
        this->pitch += yOffset;
        // make sure that when pitch is out of bounds, screen doesn't get
        // flipped
        if (constrainPitch) {
            pitch = std::min(this->pitch, 89.0f);
            pitch = std::max(this->pitch, -89.0f);
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        this->updateCameraFrame();
    }
// processes input received from a mouse scroll-wheel event. Only requires input
// on the vertical wheel-axis

    void Camera::processMouseScroll(double yOffset) {
        this->fov -= (float) yOffset * this->mouseSensitivity;
        this->fov = std::max(1.0f, this->fov);
        this->fov = std::min(80.0f, this->fov);
    }

    glm::mat4 Camera::GetPerspectiveMatrix(float winWidth, float winHeight) const {
        return glm::perspective((float) glm::radians(this->fov), winWidth / winHeight, this->zNear, this->zFar);
    }

    float Camera::GetZNear() const {
        return this->zNear;
    }

    float Camera::GetZFar() const {
        return this->zFar;
    }

    float &Camera::getZNearMeta() {
        return this->zNear;
    }

    float &Camera::getZFarMeta() {
        return this->zFar;
    }

    float &Camera::getMoveSpeedMeta() {
        return this->moveSpeed;
    }

    float &Camera::getMouseSensitivityMeta() {
        return this->mouseSensitivity;
    }

    float &Camera::getFovMeta() {
        return this->fov;
    }

    glm::vec3 Camera::GetFront() const {
        return this->front;
    }
} // namespace dawn_engine