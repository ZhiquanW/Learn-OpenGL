#include "render_window.h"

namespace dawn {
RenderWindow::RenderWindow(uint32_t w, uint32_t h, std::string name)
    : width{w}, height{h}, name{name}, last_mouse_pos(glm::vec2(0.0f, 0.0f)),
      mousePosInitialized(true), zoomModeEnabled(false) {
    glfwInit();
    // std::cout << "A" << std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    this->glfwWindow = glfwCreateWindow(this->width, this->height,
                                        this->name.c_str(), nullptr, nullptr);
    if (this->glfwWindow == NULL) {
        throw std::runtime_error("failed to create GLFW window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(this->glfwWindow);
    glfwSetInputMode(this->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
RenderWindow::~RenderWindow() {
    glfwDestroyWindow(this->glfwWindow);
    glfwTerminate();
}
uint32_t RenderWindow::get_win_width() { return this->width; }
uint32_t RenderWindow::get_win_height() { return this->height; }
bool RenderWindow::should_close() {
    return glfwWindowShouldClose(this->glfwWindow);
}

void RenderWindow::swap_buffers() { glfwSwapBuffers(this->glfwWindow); }
void RenderWindow::process_inputs(Camera *camera, GLfloat deltaTime) {
    glfwPollEvents();
    this->keyoardCallbacks(camera, deltaTime);
    this->mouseCallbacks(camera);
}
void RenderWindow::mouseCallbacks(Camera *camera) {
    GLdouble xPos, yPos;
    glfwGetCursorPos(this->glfwWindow, &xPos, &yPos);

    if (this->mousePosInitialized) {
        this->last_mouse_pos = glm::vec2(xPos, yPos);
        this->mousePosInitialized = false;
    }
    float xOffset = xPos - this->last_mouse_pos.x;
    float yOffset = this->last_mouse_pos.y - yPos;
    this->last_mouse_pos = glm::vec2(xPos, yPos);
    this->zoomModeEnabled =
        glfwGetMouseButton(this->glfwWindow, GLFW_MOUSE_BUTTON_RIGHT);
    if (this->zoomModeEnabled) {
        camera->processMouseScroll(yOffset);
        yOffset = 0.0f;
    }
    camera->ProcessMouseMovement(xOffset, yOffset);
}

void RenderWindow::keyoardCallbacks(Camera *camera, GLfloat deltaTime) {
    if (glfwGetKey(this->glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->glfwWindow, true);

    if (glfwGetKey(this->glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(this->glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(this->glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(this->glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(RIGHT, deltaTime);
    // this->zoomModeEnabled =
    //     glfwGetKey(this->glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
}

} // namespace dawn
