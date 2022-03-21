#include "render_window.h"

namespace dawn {
RenderWindow::RenderWindow(uint32_t w, uint32_t h, std::string name)
    : width{w}, height{h}, name{name} {

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
void RenderWindow::process_inputs() {
    glfwPollEvents();
    if (glfwGetKey(this->glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->glfwWindow, true);
}

} // namespace dawn
