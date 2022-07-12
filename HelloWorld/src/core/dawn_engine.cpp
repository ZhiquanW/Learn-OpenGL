#include "dawn_engine.h"
#include "shader_program_table.h"

#include <sstream>
#include <fstream>

namespace dawn_engine {
    DawnEngine *DawnEngine::instance = nullptr;

    DawnEngine::DawnEngine(uint32_t win_width, uint32_t win_height, const std::string &name)
            : deltaTime(1.0f / 60.0f), lastTime(0.0f), uiSystem(nullptr) {
        DawnEngine::instance = this;
        this->render_window_ = new RenderWindow(win_width, win_height, name);
        this->main_camera_ = Camera();
        this->game_object_ptrs = {};
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw std::runtime_error("failed to initialize glad");
            return;
        }
        glViewport(0, 0, (GLsizei) this->render_window_->GetWinWidth(),
                   (GLsizei) this->render_window_->GetWinHeight());
        this->enableFeatures();
        this->InitShaderPrograms();
    }


    DawnEngine::~DawnEngine() {
        delete this->render_window_;
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
        this->InitGlobalUniformBlocks();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        while (!this->render_window_->should_close()) {
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto currentTime = static_cast<GLfloat>(glfwGetTime());
            this->deltaTime = currentTime - this->lastTime;
            this->lastTime = currentTime;
            // Start the Dear ImGui frame
            this->render_window_->process_inputs(&this->main_camera_, this->deltaTime);
            this->update();
            this->render();
            if (uiSystem != nullptr) {
                this->uiSystem->render(this);
            }
            this->render_window_->swap_buffers();
            glfwPollEvents();

        }
    }

    void DawnEngine::AddGameObject(GameObject *gObjPtr) {
        this->game_object_ptrs.emplace_back(gObjPtr);
    }

    void DawnEngine::RefreshGlobalUniformBlocks() {
        std::vector<std::string> targetShaderPrograms = {ShaderTable::default_shader_info.name,
                                                         ShaderTable::depth_shader_info.name, ShaderTable::skybox_shader_info.name};
        // update camera uniforms
        this->uniform_buffer_map.at("CameraBlock").RefreshData(0, ExtractUniforms("main_camera", this->main_camera_, this->render_window_->GetWinWidth(),
                                                                                  this->render_window_->GetWinHeight()));
        this->uniform_buffer_map.at("CameraBlock").SyncGLData();
        size_t dir_lights_uniform_idx = 1;
        size_t point_lights_uniform_idx = 1;
        size_t spot_lights_uniform_idx = 1;
        for (auto gObj: this->game_object_ptrs) {
            auto dir_light_m = gObj->GetModule<DirectionalLightModule>();
            if (dir_light_m != nullptr) {
                auto tmp_uniforms = ExtractUniforms("", dir_light_m);
                this->uniform_buffer_map.at("DirLightBlock").RefreshData(1, tmp_uniforms);
                dir_lights_uniform_idx += tmp_uniforms.size();
                continue;
            }
            auto point_light_m = gObj->GetModule<PointLightModule>();
            if (point_light_m != nullptr) {
                auto tmp_uniforms = ExtractUniforms("", point_light_m);
                this->uniform_buffer_map.at("PointLightBlock").RefreshData(1, tmp_uniforms);
                point_lights_uniform_idx += tmp_uniforms.size();
                continue;
            }
            auto spot_light_m = gObj->GetModule<SpotLightModule>();
            if (spot_light_m != nullptr) {
                auto tmp_uniforms = ExtractUniforms("", spot_light_m);
                this->uniform_buffer_map.at("SpotLightBlock").RefreshData(1, tmp_uniforms);
                spot_lights_uniform_idx += tmp_uniforms.size();
                continue;

            }
        }
        this->uniform_buffer_map.at("DirLightBlock").SyncGLData();
        this->uniform_buffer_map.at("SpotLightBlock").SyncGLData();
        this->uniform_buffer_map.at("PointLightBlock").SyncGLData();
    }


    void DawnEngine::render() {
        this->RefreshGlobalUniformBlocks();
        std::vector<std::shared_ptr<GLRenderObject>> opaque_render_queue = {};
        std::map<float, std::shared_ptr<GLRenderObject>> transparent_render_queue = {};
        for (auto g_obj: this->game_object_ptrs) {
            if (g_obj->GetModule<RendererModule>() != nullptr) {
                float distance2cam = glm::length(g_obj->GetModule<TransformModule>()->GetPosition() - this->main_camera_.GetPosition());
                auto render_pairs = g_obj->GetModule<RendererModule>()->GetGLRenderObjectMap();
                for (auto render_pair: render_pairs) {
                    auto mesh = g_obj->GetModule<RendererModule>()->GetMesh(render_pair.first);
                    std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms = {};
                    auto transform_uniforms = ExtractUniforms("model_mat", g_obj->GetModule<TransformModule>());
                    uniforms.insert(uniforms.begin(), transform_uniforms.begin(), transform_uniforms.end());
                    if (mesh.GetMaterial().GetShaderInfo().name == ShaderTable::default_shader_info.name ||
                        mesh.GetMaterial().GetShaderInfo().name == ShaderTable::pure_shader_info.name) {
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
        // render opaque objects
        for (const auto &render_obj: opaque_render_queue) {
            render_obj->render();
        }

        // render skybox
        if (this->skybox_ptr_ != nullptr) {
            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
            skybox_ptr_->GetModule<RendererModule>()->render();
            glDepthMask(GL_TRUE);


        }
        // render transparent objects
        for (const auto &render_obj_pair: transparent_render_queue) {
            render_obj_pair.second->render();
        }
        // render opaque_ object
//        this->activeShader = this->shader_program_map.at("default");
//        for (auto gObj: this->game_object_ptrs) {
//            auto *rendererM = gObj->GetModule<RendererModule>();
////            rendererM->render();
//            auto modelMat = gObj->GetModule<TransformModule>()->GetModelMat4();
//            auto pos = gObj->GetModule<TransformModule>()->GetPosition();
//            float zDis = glm::length(this->main_camera_.GetPosition() - pos);
//            this->activeShader->activate();
//            if (rendererM and rendererM->GetActivation()) {
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


    }

    void DawnEngine::InitShaderPrograms() {
        shader_program_map.insert({ShaderTable::default_shader_info.name, new GLShaderProgram(ShaderTable::default_shader_info.name.c_str(),
                                                                                              ShaderTable::default_shader_info.vert_path.c_str(),
                                                                                              ShaderTable::default_shader_info.frag_path.c_str())});
        shader_program_map.insert({ShaderTable::depth_shader_info.name, new GLShaderProgram(ShaderTable::depth_shader_info.name.c_str(),
                                                                                            ShaderTable::depth_shader_info.vert_path.c_str(),
                                                                                            ShaderTable::depth_shader_info.frag_path.c_str())});
        shader_program_map.insert({ShaderTable::skybox_shader_info.name, new GLShaderProgram(ShaderTable::skybox_shader_info.name.c_str(),
                                                                                             ShaderTable::skybox_shader_info.vert_path.c_str(),
                                                                                             ShaderTable::skybox_shader_info.frag_path.c_str())});
        shader_program_map.insert({ShaderTable::pure_shader_info.name, new GLShaderProgram(ShaderTable::pure_shader_info.name.c_str(),
                                                                                           ShaderTable::pure_shader_info.vert_path.c_str(),
                                                                                           ShaderTable::pure_shader_info.frag_path.c_str())});

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
        this->uiSystem->initialize(this->render_window_->getWindowPtr());

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

    std::unordered_map<std::string, GLShaderProgram *> &DawnEngine::GetShaderProgramMapMeta() {
        return this->shader_program_map;
    }


    Camera &DawnEngine::GetMainCameraRef() {
        return this->main_camera_;
    }

    void DawnEngine::SetUniformInShaderPrograms(std::vector<std::string> shader_program_names, const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) {
        for (const auto &name: shader_program_names) {
            auto shaderProgramPtr = this->shader_program_map.at(name);
            if (shaderProgramPtr != nullptr) {
                shaderProgramPtr->activate();
                shaderProgramPtr->GetUniforms(uniforms);
            }
        }

    }

    void DawnEngine::AddSkybox(GameObject *skybox_ptr) {
        this->skybox_ptr_ = skybox_ptr;


    }

    void DawnEngine::awake() {
        this->InitGlobalUniformBlocks();

    }

    void DawnEngine::start() {

    }

    void DawnEngine::update() {

    }

    void DawnEngine::InitGlobalUniformBlocks() {
//        this->camera_ubo_ = AllocateUniformBlock(ExtractUniforms("main_camera", this->main_camera_, this->render_window_->GetWinWidth(), this->render_window_->GetWinHeight()));
        this->uniform_buffer_map.insert({"CameraBlock", GLUniformBuffer(0, ExtractUniforms("main_camera",
                                                                                           this->main_camera_,
                                                                                           this->render_window_->GetWinWidth(),
                                                                                           this->render_window_->GetWinHeight()))});
        // collect and classify light uniforms
        GLUniformBuffer dir_light_uniform_buffer = GLUniformBuffer(1);
        GLUniformBuffer point_light_uniform_buffer = GLUniformBuffer(2);
        GLUniformBuffer spot_light_uniform_buffer = GLUniformBuffer(3);

        int dir_lights_num = 0;
        int point_lights_num = 0;
        int spot_lights_num = 0;
        dir_light_uniform_buffer.AppendUniform(std::make_shared<ShaderUniformVariable<int>>("dir_lights_num", dir_lights_num));
        point_light_uniform_buffer.AppendUniform(std::make_shared<ShaderUniformVariable<int>>("point_lights_num", point_lights_num));
        spot_light_uniform_buffer.AppendUniform(std::make_shared<ShaderUniformVariable<int>>("spot_lights_num", spot_lights_num));
        for (auto gObj: this->game_object_ptrs) {
            auto dir_light_m = gObj->GetModule<DirectionalLightModule>();
            if (dir_light_m != nullptr) {
                dir_light_uniform_buffer.AppendStructUniforms(ExtractUniforms("", dir_light_m));
                dir_lights_num++;
                continue;
            }
            auto point_light_m = gObj->GetModule<PointLightModule>();
            if (point_light_m != nullptr) {
                point_light_uniform_buffer.AppendStructUniforms(ExtractUniforms("", point_light_m));
                point_lights_num++;
                continue;
            }
            auto spot_light_m = gObj->GetModule<SpotLightModule>();
            if (spot_light_m != nullptr) {
                spot_light_uniform_buffer.AppendStructUniforms(ExtractUniforms("", spot_light_m));
                spot_lights_num++;
                continue;

            }
        }

        dir_light_uniform_buffer.RefreshData(0, {std::make_shared<ShaderUniformVariable<int>>("dir_lights_num", dir_lights_num)});
        point_light_uniform_buffer.RefreshData(0, {std::make_shared<ShaderUniformVariable<int>>("point_lights_num", point_lights_num)});
        spot_light_uniform_buffer.RefreshData(0, {std::make_shared<ShaderUniformVariable<int>>("spot_lights_num", spot_lights_num)});
        // create uniform buffer
        // pre-compute uniform block size
        dir_light_uniform_buffer.GenBuffer();
        dir_light_uniform_buffer.SyncGLData();
        point_light_uniform_buffer.GenBuffer();
        point_light_uniform_buffer.SyncGLData();
        spot_light_uniform_buffer.GenBuffer();
        spot_light_uniform_buffer.SyncGLData();

        this->uniform_buffer_map.insert({"DirLightBlock", dir_light_uniform_buffer});
        this->uniform_buffer_map.insert({"PointLightBlock", point_light_uniform_buffer});
        this->uniform_buffer_map.insert({"SpotLightBlock", spot_light_uniform_buffer});
    }


} // namespace dawn_engine