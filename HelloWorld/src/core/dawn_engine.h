
#ifndef DAWN_ENGINE_H
#define DAWN_ENGINE_H

#include "include/game_object.h"
#include "include/render_window.h"
#include "core/dawn_ui_system.h"
#include "core/dawn_model.h"
#include "modules/transform_module.h"
#include "utils/glsl_utils.h"

namespace dawn_engine {
    class DawnEngine {
    private:
        RenderWindow *renderWindow;
        DawnUISystem *uiSystem;
        Camera mainCamera;
        bool enableDepthRendering = false;
        std::unordered_map<std::string, DawnModel> modelMap = {};
        std::unordered_map<std::string, OpenGLShaderProgram *> shaderProgramMap = {};
        OpenGLShaderProgram *activeShader;
        GLfloat deltaTime;
        GLfloat lastTime;

        // customized data
        void render();


    protected:
        std::vector<GameObject *> gameObjectPtrs;

        void mountUISystem(DawnUISystem *uiSystem);

        virtual void awake() = 0;

        virtual void start() = 0;

        virtual void update() = 0;

        virtual void uniformUpdate();

        void addGameObject(GameObject *gObjPtr);

    public:

        DawnEngine(uint32_t win_width, uint32_t win_height, const std::string &name);

        DawnEngine(const DawnEngine &) = delete;

        ~DawnEngine();


        void launch();

        void add_data();

        void initShaderPrograms();

        void setUniformInShaderPrograms(std::vector<std::string> shaderProgramNames, const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms);

//        void loadTextures(const char *, const char *);

        std::vector<GameObject *> getGameObjectPtrs() const;

        void loadModel(const std::string &modelPath);

        bool enabledDepthRendering() const;

        bool &getDepthRenderingSwitchMeta();

        std::unordered_map<std::string, OpenGLShaderProgram *> &getShaderProgramMapMeta();

        void setActiveShaderProgram(const char *name);

        [[maybe_unused]] OpenGLShaderProgram *getActiveShaderProgram();

        Camera &getMainCameraMeta();
    };

} // namespace dawn_engine

#endif //DAWN_ENGINE_H
