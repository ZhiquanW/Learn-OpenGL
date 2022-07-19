
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
        DawnUISystem *ui_system_;

        void render_depth_map();
        // customized data
        void render();

        std::unordered_map<std::string, GLUniformBuffer> uniform_buffer_map = {};

    protected:
        Camera main_camera_;
        bool enableDepthRendering = false;
        std::unordered_map<std::string, DawnModel> modelMap = {};
        std::unordered_map<std::string, GLShaderProgram *> shader_program_map = {};
        std::unordered_map<std::string, DawnMaterial> material_map = {};
        GLfloat deltaTime;
        GLfloat lastTime;
        std::vector<GameObject *> game_object_ptrs;
        GameObject *skybox_ptr_;
        unsigned int depth_fbo = 0;

        void MountUISystem(DawnUISystem *ui_system);

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

        void InitMaterials();

        void EnableGLFeatures();

        void SetUniformInShaderPrograms(const std::vector<std::string>& shader_program_names, const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms);

        std::vector<GameObject *> GetGameObjectPtrs() const;

        bool enabledDepthRendering() const;

        bool &getDepthRenderingSwitchMeta();

        std::unordered_map<std::string, GLShaderProgram *> &GetShaderProgramMapMeta();

        Camera &GetMainCameraRef();

        RayHitInfo RayCastDetection(Ray ray);

        GameObject * FindGameObjectByName(std::string name);

        GameObject  * CreatePrimitive(PrimitiveType pType) ;

        void AddMaterial(const std::string & name, const DawnMaterial& material) ;

        DawnMaterial GetMaterial(const std::string & name);

        void AddShaderProgram(const std::string & name, GLShaderProgram * shader_program);

        GLShaderProgram * GetShaderProgram(const std::string & name);
    };

} // namespace dawn_engine

#endif //DAWN_ENGINE_H
