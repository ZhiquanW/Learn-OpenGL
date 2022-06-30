
#ifndef DAWN_ENGINE_H
#define DAWN_ENGINE_H

#include "game_object.h"
#include "include/render_window.h"
#include "core/dawn_ui_system.h"
#include "core/dawn_model.h"
#include "modules/transform_module.h"
#include "utils/glsl_utils.h"
#include "graphics_layer/gl_render_object.h"

namespace dawn_engine {
    class DawnEngine {
    private:
        RenderWindow *renderWindow;
        DawnUISystem *uiSystem;
        Camera mainCamera;
        bool enableDepthRendering = false;
        std::unordered_map<std::string, DawnModel> modelMap = {};
        std::unordered_map<std::string, GLShaderProgram *> shaderProgramMap = {};
        GLShaderProgram *activeShader;
        GLfloat deltaTime;
        GLfloat lastTime;

        // customized data
        void render();


    protected:
        std::vector<GameObject *> game_object_ptrs;
        GameObject *skyboxPtr;

        void mountUISystem(DawnUISystem *uiSystem);

        virtual void awake();

        virtual void start();

        virtual void update();

        virtual void GlobalUniformRefresh();

        void addGameObject(GameObject *gObjPtr);

        void addSkybox(GameObject *skyboxPtr);

    public:
        static DawnEngine *engineInstance;

        DawnEngine(uint32_t win_width, uint32_t win_height, const std::string &name);

        DawnEngine(const DawnEngine &) = delete;

        DawnEngine &operator=(const DawnEngine &) = delete;

        ~DawnEngine();


        void launch();

        void initShaderPrograms();

        void enableFeatures();

        void setUniformInShaderPrograms(std::vector<std::string> shaderProgramNames, const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms);

        std::vector<GameObject *> getGameObjectPtrs() const;

        bool enabledDepthRendering() const;

        bool &getDepthRenderingSwitchMeta();

        std::unordered_map<std::string, GLShaderProgram *> &getShaderProgramMapMeta();

        void setActiveShaderProgram(const char *name);

        [[maybe_unused]] GLShaderProgram *getActiveShaderProgram();

        Camera &getMainCameraMeta();

//        static GLShaderProgram *findShaderProgram(std::string name);
    };

} // namespace dawn_engine

#endif //DAWN_ENGINE_H
