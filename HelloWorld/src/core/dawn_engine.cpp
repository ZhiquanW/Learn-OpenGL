#include "dawn_engine.h"
#include "shader_program_table.h"

#include <sstream>
#include <fstream>

namespace dawn_engine {
    DawnEngine *DawnEngine::engineInstance = nullptr;

    DawnEngine::DawnEngine(uint32_t win_width, uint32_t win_height, const std::string &name)
            : deltaTime(1.0f / 60.0f), lastTime(0.0f), uiSystem(nullptr) {
        DawnEngine::engineInstance = this;
        this->renderWindow = new RenderWindow(win_width, win_height, name);
        this->mainCamera = Camera();
        this->game_object_ptrs = {};
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw std::runtime_error("failed to initialize glad");
            return;
        }
        glViewport(0, 0, (GLsizei) this->renderWindow->getWinWidth(),
                   (GLsizei) this->renderWindow->getWinHeight());
        this->enableFeatures();
        this->initShaderPrograms();
    }


    DawnEngine::~DawnEngine() {
        delete this->renderWindow;
        if (!this->uiSystem) {
            delete this->uiSystem;
        }
    }

    void DawnEngine::launch() {
        this->awake();
        this->start();
        if (this->uiSystem != nullptr) {
            this->uiSystem->start(this);
        }
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//        glClearDepth(1.0f);
        while (!this->renderWindow->should_close()) {
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto currentTime = static_cast<GLfloat>(glfwGetTime());
            this->deltaTime = currentTime - this->lastTime;
            this->lastTime = currentTime;
            // Start the Dear ImGui frame
            this->renderWindow->process_inputs(&this->mainCamera, this->deltaTime);
            this->update();
            this->render();
            if (uiSystem != nullptr) {
                this->uiSystem->render(this);
            }
            this->renderWindow->swap_buffers();
            glfwPollEvents();

        }
    }

    void DawnEngine::addGameObject(GameObject *gObjPtr) {
        this->game_object_ptrs.emplace_back(gObjPtr);
    }

    void DawnEngine::GlobalUniformRefresh() {
        std::vector<std::string> targetShaderPrograms = {ShaderTable::default_shader_info.name,
                                                         ShaderTable::depth_shader_info.name, ShaderTable::skybox_shader_info.name};
        // update camera uniforms
        this->setUniformInShaderPrograms(targetShaderPrograms, ExtractUniforms("main_camera", mainCamera, this->renderWindow->getWinWidth(),
                                                                               this->renderWindow->getWinHeight()));
        this->setUniformInShaderPrograms({ShaderTable::skybox_shader_info.name},
                                         {std::make_shared<ShaderUniformVariable<glm::mat4>>("main_camera.view", glm::mat4(glm::mat3(this->mainCamera.GetViewMatrix())))});
        // update light uniforms
        uint32_t dirLightNum = 0;
        uint32_t pointLightNum = 0;
        uint32_t spotLightNum = 0;
        for (auto gObj: this->game_object_ptrs) {
            auto *dirLightM = gObj->GetModule<DirectionalLightModule>();
            if (dirLightM != nullptr && dirLightM->getActivation()) {
                this->setUniformInShaderPrograms(targetShaderPrograms, ExtractUniforms(fmt::format("directional_lights[{}]", dirLightNum++), dirLightM));
            }
            auto *pointLightM = gObj->GetModule<PointLightModule>();
            if (pointLightM != nullptr && pointLightM->getActivation()) {
                this->setUniformInShaderPrograms(targetShaderPrograms, ExtractUniforms(fmt::format("point_lights[{}]", pointLightNum++), pointLightM));
            }
            auto *spotLightM = gObj->GetModule<SpotLightModule>();
            if (spotLightM != nullptr && spotLightM->getActivation()) {
                this->setUniformInShaderPrograms(targetShaderPrograms, ExtractUniforms(fmt::format("spot_lights[{}]", spotLightNum++), spotLightM));
            }
        }
        this->setUniformInShaderPrograms(targetShaderPrograms, {std::make_shared<ShaderUniformVariable<int>>("dir_lights_num", int(dirLightNum)),
                                                                std::make_shared<ShaderUniformVariable<int>>("point_lights_num", int(pointLightNum)),
                                                                std::make_shared<ShaderUniformVariable<int>>("spot_lights_num", int(spotLightNum))});
        //
    }


    void DawnEngine::render() {
        this->GlobalUniformRefresh();
        // sort objects for transparent rendering
        std::vector<std::shared_ptr<GLRenderObject>> opaque_render_queue = {};
        std::map<float, std::shared_ptr<GLRenderObject>> transparent_render_queue = {};
        for (auto g_obj: this->game_object_ptrs) {
            if (g_obj->GetModule<RendererModule>() != nullptr) {
                float distance2cam = glm::length(g_obj->GetModule<TransformModule>()->GetPosition() - this->mainCamera.GetPosition());
                auto render_pairs = g_obj->GetModule<RendererModule>()->GetGLRenderObjectMap();
                for (auto render_pair: render_pairs) {
                    auto mesh = g_obj->GetModule<RendererModule>()->GetMesh(render_pair.first);
                    std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms = {};
                    auto transform_uniforms = ExtractUniforms("model_mat", g_obj->GetModule<TransformModule>());
                    uniforms.insert(uniforms.begin(), transform_uniforms.begin(), transform_uniforms.end());
                    if (mesh.GetMaterial().GetShaderInfo().name == ShaderTable::default_shader_info.name) {
                        auto material_uniforms = ExtractUniforms("material", mesh.GetMaterial());
                        uniforms.insert(uniforms.begin(), material_uniforms.begin(), material_uniforms.end());
                    }
                    render_pair.second->RefreshUniforms(uniforms);
                    if (mesh.GetMaterial().GetOpaque()) {
                        opaque_render_queue.push_back(render_pair.second);
                    } else {
                        transparent_render_queue.insert({distance2cam, render_pair.second});
                    }
                }
            }
        }
        // render opqaue objects
        for (const auto &render_obj: opaque_render_queue) {
            render_obj->render();
        }
        // render transparent objects
        for (const auto &render_obj_pair: transparent_render_queue) {
            render_obj_pair.second->render();
        }
        // render opaque_ object
//        this->activeShader = this->shaderProgramMap.at("default");
//        for (auto gObj: this->game_object_ptrs) {
//            auto *rendererM = gObj->GetModule<RendererModule>();
////            rendererM->render();
//            auto modelMat = gObj->GetModule<TransformModule>()->GetModelMat4();
//            auto pos = gObj->GetModule<TransformModule>()->GetPosition();
//            float zDis = glm::length(this->mainCamera.GetPosition() - pos);
//            this->activeShader->activate();
//            if (rendererM and rendererM->getActivation()) {
//                for (const auto &mesh: rendererM->getMeshesRef()) {
//                    if (mesh.GetMaterial().GetOpaque()) {
//                        // render opaque_ object
//                        this->activeShader->setUniform("model_mat", gObj->GetModule<TransformModule>()->GetModelMat4());
//                        mesh.render(this->activeShader);
//                    } else {
//                        // insert transparent object for sorting
//                        transparentRenderMap.insert({zDis, {mesh, modelMat}});
//                    }
//                }
//            }
//        }
        // render transparent object
//        for (auto it = transparentRenderMap.rbegin(); it != transparentRenderMap.rend(); ++it) {
//            this->activeShader->setUniform("model_mat", it->second.modelMat);
//            it->second.mesh.render(this->activeShader);
//        }
        // render skybox
        if (this->skyboxPtr != nullptr) {
            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
            skyboxPtr->GetModule<RendererModule>()->render();
            glDepthMask(GL_TRUE);


        }

    }

    void DawnEngine::initShaderPrograms() {
        shaderProgramMap.insert({ShaderTable::default_shader_info.name, new GLShaderProgram(ShaderTable::default_shader_info.name.c_str(),
                                                                                            ShaderTable::default_shader_info.vert_path.c_str(),
                                                                                            ShaderTable::default_shader_info.frag_path.c_str())});
        shaderProgramMap.insert({ShaderTable::depth_shader_info.name, new GLShaderProgram(ShaderTable::depth_shader_info.name.c_str(),
                                                                                          ShaderTable::depth_shader_info.vert_path.c_str(),
                                                                                          ShaderTable::depth_shader_info.frag_path.c_str())});
        shaderProgramMap.insert({ShaderTable::skybox_shader_info.name, new GLShaderProgram(ShaderTable::skybox_shader_info.name.c_str(),
                                                                                           ShaderTable::skybox_shader_info.vert_path.c_str(),
                                                                                           ShaderTable::skybox_shader_info.frag_path.c_str())});

//        shaderProgramMap.insert({"default", new GLShaderProgram("default", "../shaders/default_rendering.vert",
//                                                                "../shaders/default_rendering.frag")});
//        shaderProgramMap.insert({"depth", new GLShaderProgram("depth", "../shaders/default_rendering.vert",
//                                                              "../shaders/depth_buffer_rendering.frag")});
//        shaderProgramMap.insert({"skybox", new GLShaderProgram("depth", "../shaders/skybox.vert",
//                                                               "../shaders/skybox.frag")});
        this->activeShader = shaderProgramMap["default"];
    }

    void DawnEngine::enableFeatures() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);


    }

    void DawnEngine::mountUISystem(DawnUISystem *uiSystem) {
        this->uiSystem = uiSystem;
        this->uiSystem->initialize(this->renderWindow->getWindowPtr());

    }

    std::vector<GameObject *> DawnEngine::getGameObjectPtrs() const {
        return this->game_object_ptrs;
    }


    bool DawnEngine::enabledDepthRendering() const {
        return this->enableDepthRendering;
    }

    bool &DawnEngine::getDepthRenderingSwitchMeta() {
        return this->enableDepthRendering;
    }

    std::unordered_map<std::string, GLShaderProgram *> &DawnEngine::getShaderProgramMapMeta() {
        return this->shaderProgramMap;
    }

    void DawnEngine::setActiveShaderProgram(const char *name) {
        this->activeShader = this->shaderProgramMap.at(name);

    }

    [[maybe_unused]] GLShaderProgram *DawnEngine::getActiveShaderProgram() {
        return this->activeShader;


    }

    Camera &DawnEngine::getMainCameraMeta() {
        return this->mainCamera;
    }

    void DawnEngine::setUniformInShaderPrograms(std::vector<std::string> shaderProgramNames, const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) {
        for (const auto &name: shaderProgramNames) {
            auto shaderProgramPtr = this->shaderProgramMap.at(name);
            if (shaderProgramPtr != nullptr) {
                shaderProgramPtr->activate();
                shaderProgramPtr->setUniforms(uniforms);
            }
        }

    }

    void DawnEngine::addSkybox(GameObject *skyboxPtr) {
        this->skyboxPtr = skyboxPtr;


    }

    void DawnEngine::awake() {

    }

    void DawnEngine::start() {

    }

    void DawnEngine::update() {

    }


} // namespace dawn_engine