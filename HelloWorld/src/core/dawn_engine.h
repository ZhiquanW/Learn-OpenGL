
#ifndef DAWN_ENGINE_H
#define DAWN_ENGINE_H

//#include "include/common_includes.h"
//#include "include/render_window.h"
//#include "include/opengl_shader_program.h"
//#include "include/camera.h"
//#include "include/material.h"
//#include "include/game_object.h"
#include "include/render_window.h"
#include "dawn_ui_system.h"
#include "dawn_model.h"
#include "modules/light_module.h"
//#include "modules/transform_module.h.h"

namespace dawn_engine {
    class DawnEngine {
    private:
        RenderWindow *renderWindow;
        DawnUISystem *uiSystem;
        Camera mainCamera;
        std::unordered_map<std::string, DawnModel> modelMap = {};
        OpenGLShaderProgram *gameObjectShader;
        OpenGLShaderProgram *lightShader;
        OpenGLShaderProgram *testShader;
        OpenGLShaderProgram *modelShader;
        unsigned int VAO, VBO;
        float timeValue;
        GLfloat deltaTime;
        GLfloat lastTime;
        const uint32_t MAX_DIR_LIGHT_NUM = 16;
        const uint32_t MAX_SPOT_LIGHT_NUM = 16;
        const uint32_t MAX_POINT_LIGHT_NUM = 16;
        // customized data

        unsigned int cubeVAO, cubeVBO, cubeEBO;
        unsigned int lightVAO, lightVBO, lightEBO;
        unsigned int texture0;
        unsigned int texture1;
        glm::vec3 lightColor;
        glm::vec3 objectColor;
        glm::vec3 ambientColor;

        // void addDefaultLight();
        // void addDefaultCube();
        void render();


    protected:
        std::vector<GameObject *> gameObjectPtrs;

        void mountUISystem(DawnUISystem *uiSystem);

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

        DawnEngine(uint32_t win_width, uint32_t win_height, const std::string &name);

        DawnEngine(const DawnEngine &) = delete;

        ~DawnEngine();


        void launch();


        void add_data();

        void createShaderPrograms();

        void loadTextures(const char *, const char *);

        std::vector<GameObject *> getGameObjectPtrs() const;

        void loadModel(const std::string &modelPath);
    };

} // namespace dawn_engine

#endif //DAWN_ENGINE_H
