#pragma once
#include <GLFW/glfw3.h>
#include "common_includes.h"
namespace dawn {
class RenderWindow {
  private:
    GLFWwindow *glfwWindow;
    uint32_t width;
    uint32_t height;
    std::string name;

  public:
    RenderWindow(uint32_t w, uint32_t h, std::string);
    ~RenderWindow();
    RenderWindow(const RenderWindow &) = delete;
    RenderWindow &operator=(const RenderWindow &) = delete;
    uint32_t get_win_width();
    uint32_t get_win_height();
    bool should_close();
    void swap_buffers();
    void process_inputs();
};

} // namespace dawn