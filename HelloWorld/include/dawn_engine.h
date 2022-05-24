#pragma once

#include "common_includes.h"
#include "render_window.h"
#include "dawn_shader_program.h"
#include "camera.h"
#include "material.h"
#include "light_module.h"
#include "game_object.h"
#include "dawn_ui_manager.h"
namespace dawn_engine {
class DawnEngine {
  private:
    RenderWindow *renderWindow;
    DawnUIManager *uiManager;
    Camera mainCamera;
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
    glm::vec3 lightColor;
    glm::vec3 objectColor;
    glm::vec3 ambientColor;
    void render();
    // void addDefaultLight();
    // void addDefaultCube();

  protected:
    virtual void awake() = 0;
    virtual void start() = 0;
    virtual void update() = 0;
    // void addGameObject(const std::shared_ptr<GameObject> gameObjPtr);
    void addGameObject(GameObject *gObjPtr);
    GameObject *createGameObject();
    GameObject *createGameObject(bool isEntity);
    GameObject *createGameObject(bool isEntity, glm::vec3 position);
    GameObject *createLight(LightType lType);
    GameObject *setupLights();

  public:
    std::vector<GameObject *> gameObjectPtrs;

    DawnEngine(uint32_t win_width, uint32_t win_height, const std::string name);
    DawnEngine(const DawnEngine &) = delete;

    ~DawnEngine();
    void launch();
    void add_data();
    void createShaderPrograms();
    void loadTextures(const char *, const char *);
};

} // namespace dawn_engine