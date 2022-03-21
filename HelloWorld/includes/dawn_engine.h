#include <glad/glad.h>
#include "render_window.h"
#include "common_includes.h"
namespace dawn {
class DawnEngine {
  private:
    RenderWindow *renderWindow;
    unsigned int vao, vbo, ebo;
    unsigned int shaderProgram;

  public:
    DawnEngine(uint32_t win_width, uint32_t win_height);
    ~DawnEngine();
    void launch();
    void add_data();
    void create_shader();
};
} // namespace dawn