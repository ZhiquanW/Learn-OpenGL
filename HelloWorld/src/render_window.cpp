#include "render_window.h"

#include <utility>

namespace dawn_engine {
    void windowReshapeCallbacks(GLFWwindow *window, int w, int h) {
        glViewport(0, 0, w, h);
    }

    RenderWindow::RenderWindow(uint32_t w, uint32_t h, std::string name)
            : width{w}, height{h}, name{std::move(name)}, last_mouse_pos(glm::vec2(0.0f, 0.0f)),
              mousePosInitialized(false),
              mouseRightButtonDown(false) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        this->glfwWindow = glfwCreateWindow((int) this->width, (int) this->height,
                                            this->name.c_str(), nullptr, nullptr);
        int ww, hh;
        glfwGetFramebufferSize(this->glfwWindow, &ww, &hh);
        if (this->glfwWindow == nullptr) {
            throw std::runtime_error("failed to create GLFW window");
//        glfwTerminate();
//        return;
        }
        glfwMakeContextCurrent(this->glfwWindow);
        glfwSetFramebufferSizeCallback(this->glfwWindow, windowReshapeCallbacks);
    }

    RenderWindow::~RenderWindow() {
        glfwDestroyWindow(this->glfwWindow);
        glfwTerminate();
    }

    uint32_t RenderWindow::getWinWidth() const { return this->width; }

    uint32_t RenderWindow::getWinHeight() const { return this->height; }

    GLFWwindow *RenderWindow::getWindowPtr() const { return this->glfwWindow; }

    bool RenderWindow::should_close() { return glfwWindowShouldClose(this->glfwWindow); }

    void RenderWindow::swap_buffers() { glfwSwapBuffers(this->glfwWindow); }

    void RenderWindow::process_inputs(Camera *camera, GLfloat deltaTime) {
        int w, h;
        glfwGetFramebufferSize(this->glfwWindow, &w, &h);
        this->width = w;
        this->height = h;
        this->mouseCallbacks(camera);
        this->keyboardCallbacks(camera, deltaTime);
    }

    void RenderWindow::mouseCallbacks(Camera *camera) {
        GLdouble xPos, yPos;
        glfwGetCursorPos(this->glfwWindow, &xPos, &yPos);
        if (not this->mousePosInitialized) {
            this->last_mouse_pos = glm::vec2(xPos, yPos);
            this->mousePosInitialized = true;
        }
        GLdouble xOffset = xPos - this->last_mouse_pos.x;
        GLdouble yOffset = this->last_mouse_pos.y - yPos;
        this->last_mouse_pos = glm::vec2(xPos, yPos);
        this->mouseRightButtonDown = glfwGetMouseButton(this->glfwWindow,
                                                        GLFW_MOUSE_BUTTON_RIGHT);
        if (this->mouseRightButtonDown) {
            glfwSetInputMode(this->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // camera->processMouseScroll(yOffset);
            // yOffset = 0.0f;
            camera->processMouseMovement(xOffset, yOffset);
        } else {
            glfwSetInputMode(this->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    void RenderWindow::keyboardCallbacks(Camera *camera, GLfloat deltaTime) {
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
        // this->mouseRightButtonDown =
        //     glfwGetKey(this->glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
    }


} // namespace dawn_engine
