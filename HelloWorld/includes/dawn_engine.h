#include "common_includes.h"
#include "render_window.h"
#include "dawn_shader_program.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include "game_object.h"
namespace dawn {
class DawnEngine {
  private:
    RenderWindow *renderWindow;
    Camera mainCamera;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::vector<std::shared_ptr<Light>> lights;

    DawnShaderProgram *gameObjectShader;
    DawnShaderProgram *lightShader;
    DawnShaderProgram *testShader;
    unsigned int VAO, VBO;
    float timeValue;
    GLfloat deltaTime;
    GLfloat lastTime;
    // customized data

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    unsigned int lightVAO, lightVBO, lightEBO;
    unsigned int texture0;
    unsigned int texture1;
    Material tcubeMaterial;
    DirectionalLight dLight0;
    glm::vec3 lightColor;
    glm::vec3 objectColor;
    glm::vec3 ambientColor;
    void render();
    void addDefaultLight();
    void addDefaultCube();

  protected:
    void awake();
    void start();
    void update();
    void addGameObject(bool isEntity);
    void addGameObject(bool isEntity, glm::vec3 position);
    void setupLights();

  public:
    DawnEngine(uint32_t win_width, uint32_t win_height, const std::string name);
    ~DawnEngine();
    void launch();
    void addGameObject();
    void add_data();
    void addLight(DirectionalLight light);
    void addLight(PointLight light);
    void createShaderPrograms();
    void loadTextures(const char *, const char *);
};
} // namespace dawn