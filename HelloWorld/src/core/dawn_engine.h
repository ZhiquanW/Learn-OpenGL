
#ifndef DAWN_ENGINE_H
#define DAWN_ENGINE_H

#include "modules/collider_module.h"
#include "game_object.h"
#include "include/render_window.h"
#include "modules/transform_module.h"
#include "core/dawn_ui_system.h"
#include "core/dawn_model.h"
#include "utils/glsl_utils.h"
#include "graphics_layer/gl_render_object.h"
#include "graphics_layer/gl_uniform_buffer.h"

namespace dawn_engine {
    class DawnEngine {
    private:
        DawnUISystem *uiSystem;

        // customized data
        void render();

        std::unordered_map<std::string, GLUniformBuffer> uniform_buffer_map = {};

    protected:
        Camera main_camera_;
        bool enableDepthRendering = false;
        std::unordered_map<std::string, DawnModel> modelMap = {};
        std::unordered_map<std::string, GLShaderProgram *> shader_program_map = {};
        GLfloat deltaTime;
        GLfloat lastTime;
        std::vector<GameObject *> game_object_ptrs;
        GameObject *skybox_ptr_;

        void mountUISystem(DawnUISystem *uiSystem);

        virtual void awake();

        virtual void start();

        virtual void update();

        virtual void InitGlobalUniformBlocks();

        virtual void RefreshGlobalUniformBlocks();


    public:
        RenderWindow *render_window_;

        static DawnEngine *instance;

        DawnEngine(uint32_t win_width, uint32_t win_height, const std::string &name);

        DawnEngine(const DawnEngine &) = delete;

        DawnEngine &operator=(const DawnEngine &) = delete;

        ~DawnEngine();


        void launch();

        void AddGameObject(GameObject *gObjPtr);

        void AddSkybox(GameObject *skybox_ptr);


        void InitShaderPrograms();

        void enableFeatures();

        void SetUniformInShaderPrograms(std::vector<std::string> shader_program_names, const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms);

        std::vector<GameObject *> GetGameObjectPtrs() const;

        bool enabledDepthRendering() const;

        bool &getDepthRenderingSwitchMeta();

        std::unordered_map<std::string, GLShaderProgram *> &GetShaderProgramMapMeta();


        [[maybe_unused]] GLShaderProgram *getActiveShaderProgram();

        Camera &GetMainCameraRef();

        RayHitInfo RayCastDetection(Ray ray);

//        static GLShaderProgram *findShaderProgram(std::string name);
    };

} // namespace dawn_engine

#endif //DAWN_ENGINE_H
