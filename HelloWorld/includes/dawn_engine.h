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

    DawnShaderProgram *gameObjectShader;
    DawnShaderProgram *lightShader;
    DawnShaderProgram *testShader;
    unsigned int VAO, VBO;
    float timeValue;
    GLfloat deltaTime;
    GLfloat lastTime;
    const uint32_t MAX_DIR_LIGHT_NUM = 64;
    const uint32_t MAX_POINT_LIGHT_NUM = 64;
    const uint32_t MAX_FLASH_LIGHT_NUM = 64;
    uint32_t dirLightNum = 0;
    uint32_t pointLightNum = 0;
    uint32_t flashLightNum = 0;
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
    virtual void awake() = 0;
    virtual void start() = 0;
    virtual void update() = 0;
    void addGameObject(const std::shared_ptr<GameObject> gameObjPtr);
    // void addGameObject(bool isEntity, glm::vec3 position);
    void setupLights();

  public:
    DawnEngine(uint32_t win_width, uint32_t win_height, const std::string name);
    ~DawnEngine();
    void launch();
    void add_data();
    void addLight(const std::shared_ptr<DirectionalLight> light);
    void addLight(const std::shared_ptr<PointLight> light);
    void createShaderPrograms();
    void loadTextures(const char *, const char *);
};
} // namespace dawn