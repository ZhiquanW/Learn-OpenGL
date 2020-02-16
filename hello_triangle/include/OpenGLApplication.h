#ifndef OPENGLAPPLICATION
#define OPENGLAPPLICATION
#include "ShaderProgram.h"
#include <GLFW/glfw3.h>
class OpenGLApplication {
  public:
    OpenGLApplication();
    void run();
    bool init_application(int, int);
    void add_vao(unsigned int);

  private:
    GLFWwindow *window;
    ShaderProgram *shader_program;
    std::vector<unsigned int> vao_list;
    bool init_window(int, int);
    void set_render_objects();
    bool init_glad();
    void draw_frame();
    void main_loop();
    void cleanup();
};

#endif