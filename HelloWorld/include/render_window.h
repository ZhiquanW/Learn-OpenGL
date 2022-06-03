#pragma once
#include "common_includes.h"
#include "camera.h"

namespace dawn_engine {
class RenderWindow {
  private:
    GLFWwindow *glfwWindow;
    uint32_t width;
    uint32_t height;
    glm::vec2 last_mouse_pos;
    bool mousePosInitialized;
    bool mouseRightButtonDown;
    std::string name;
    void mouseCallbacks(Camera *camera);
    void keyboardCallbacks(Camera *camera, GLfloat deltaTime);

  public:
    RenderWindow(uint32_t w, uint32_t h, std::string );
    ~RenderWindow();
    RenderWindow(const RenderWindow &) = delete;
    RenderWindow &operator=(const RenderWindow &) = delete;
    [[nodiscard]] uint32_t getWinWidth() const;
    [[nodiscard]] uint32_t getWinHeight() const;
    bool should_close();
    void swap_buffers();
    void process_inputs(Camera *camera, GLfloat deltaTime);
    [[nodiscard]] GLFWwindow *getWindowPtr() const;
};

} // namespace dawn_engine