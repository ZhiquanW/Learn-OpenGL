#include <GLFW/glfw3.h>

class OpenGLApplication {
public:
  void run();
  bool init_application(int, int);

private:
  GLFWwindow *window;
  bool init_window(int, int);
  bool init_glad();
  void main_loop();
  void cleanup();
};