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
    bool zoomModeEnabled;
    std::string name;
    void mouseCallbacks(Camera *camera);
    void keyoardCallbacks(Camera *camera, GLfloat deltaTime);

  public:
    RenderWindow(uint32_t w, uint32_t h, std::string);
    ~RenderWindow();
    RenderWindow(const RenderWindow &) = delete;
    RenderWindow &operator=(const RenderWindow &) = delete;
    uint32_t get_win_width();
    uint32_t get_win_height();
    bool should_close();
    void swap_buffers();
    void process_inputs(Camera *camera, GLfloat deltaTime);
};

} // namespace dawn_engine