#include "dawn_engine.h"
#include "shader_program_table.h"

#include <sstream>
#include <fstream>

namespace dawn_engine {
    DawnEngine *DawnEngine::instance = nullptr;

    DawnEngine::DawnEngine(uint32_t win_width, uint32_t win_height, const std::string &name)
            : deltaTime(1.0f / 60.0f), lastTime(0.0f), ui_system_(nullptr) {
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
        this->EnableGLFeatures();
        this->InitShaderPrograms();
        this->InitMaterials();
        // TMP: generate depth texture and framebuffer
        this->depth_fbo = AllocateGLDepthMap(glm::vec2(1024));
    }


    DawnEngine::~DawnEngine() {
        delete this->render_window_;
        if (!this->ui_system_) {
            delete this->ui_system_;
        }
    }

    void DawnEngine::launch() {
        this->awake();
        this->start();
        if (this->ui_system_ != nullptr) {
            this->ui_system_->start(this);
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
            if (ui_system_ != nullptr) {
                this->ui_system_->render(this);
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


    /*  render depth of scene to texture (from light's perspective)
     * 1. set light uniform
     * 2. activate light depth shader
     * 3. set light uniforms
     * 4. render
    */
    void DawnEngine::render_depth_map() {
        auto light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, main_camera_.GetZNear(),main_camera_.GetZFar());
        auto dir_light = this->FindGameObjectByName("dir_light");
        auto light_view = glm::lookAt(glm::vec3(10.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        auto light_space_mat = light_projection * light_view;
        auto depth_shader = this->shader_program_map.at("simple_depth");
        glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, this->depth_fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        std::vector<std::shared_ptr<GLRenderObject>> opaque_render_queue = {};
        std::map<float, std::shared_ptr<GLRenderObject>> transparent_render_queue = {};
        for (auto g_obj: this->game_object_ptrs) {
            if (g_obj->GetModule<RendererModule>() != nullptr) {
                float distance2cam = glm::length(g_obj->GetModule<TransformModule>()->GetPosition() - this->main_camera_.GetPosition());
                auto render_pairs = g_obj->GetModule<RendererModule>()->GetGLRenderObjectMap();
                for (const auto& render_pair: render_pairs) {
                    auto mesh = g_obj->GetModule<RendererModule>()->GetMesh(render_pair.first);

                    std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms = {std::make_shared<ShaderUniformVariable<glm::mat4 >>("light_space_mat", light_space_mat)};
                    auto transform_uniforms = ExtractUniforms("model_mat", g_obj->GetModule<TransformModule>());
                    uniforms.insert(uniforms.begin(), transform_uniforms.begin(), transform_uniforms.end());
                    render_pair.second->RefreshShaderProgram(depth_shader);
                    render_pair.second->RefreshUniforms(uniforms);

                    if (mesh.GetMaterialPtr()->GetOpaque()) {
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
        // render transparent objects
        for (const auto &render_obj_pair: transparent_render_queue) {
            render_obj_pair.second->render();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    void DawnEngine::render() {
        this->RefreshGlobalUniformBlocks();
        this->render_depth_map();
        // main render
        // reset viewport
        glViewport(0, 0, (GLsizei) this->render_window_->GetWinWidth(),
                   (GLsizei) this->render_window_->GetWinHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        std::vector<std::shared_ptr<GLRenderObject>> opaque_render_queue = {};
        std::map<float, std::shared_ptr<GLRenderObject>> transparent_render_queue = {};
        for (auto g_obj: this->game_object_ptrs) {
            if (g_obj->GetModule<RendererModule>() != nullptr) {
                float distance2cam = glm::length(g_obj->GetModule<TransformModule>()->GetPosition() - this->main_camera_.GetPosition());
                auto render_pairs = g_obj->GetModule<RendererModule>()->GetGLRenderObjectMap();
                for (const auto& render_pair: render_pairs) {
                    auto mesh = g_obj->GetModule<RendererModule>()->GetMesh(render_pair.first);
                    std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms = {};
                    auto transform_uniforms = ExtractUniforms("model_mat", g_obj->GetModule<TransformModule>());
                    uniforms.insert(uniforms.begin(), transform_uniforms.begin(), transform_uniforms.end());
                    if (mesh.GetMaterialPtr()->GetShaderInfo().name == ShaderTable::default_shader_info.name ||
                            mesh.GetMaterialPtr()->GetShaderInfo().name == ShaderTable::pure_shader_info.name) {
                        auto material_uniforms = ExtractUniforms("material", mesh.GetMaterialPtr());
                        uniforms.insert(uniforms.begin(), material_uniforms.begin(), material_uniforms.end());
                    }
                    render_pair.second->RefreshUniforms(uniforms);
                    render_pair.second->RefreshShaderProgram(this->shader_program_map.at(mesh.GetMaterialPtr()->GetShaderInfo().name));
                    if (mesh.GetMaterialPtr()->GetOpaque()) {
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
//                    if (mesh.GetMaterialPtr().GetOpaque()) {
//                        // render opaque_ object
//                        this->activeShader->SetUniform("model_mat", gObj->GetModule<TransformModule>()->GetModelMat4());
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
//            this->activeShader->SetUniform("model_mat", it->second.modelMat);
//            it->second.mesh.render(this->activeShader);
//        }


    }

    void DawnEngine::InitShaderPrograms() {
        shader_program_map.insert({ShaderTable::default_shader_info.name, new GLShaderProgram(ShaderTable::default_shader_info.name.c_str(),
                                                                                              ShaderTable::blinn_phong_info.vert_path.c_str(),
                                                                                              ShaderTable::blinn_phong_info.frag_path.c_str())});
        shader_program_map.insert({ShaderTable::depth_shader_info.name, new GLShaderProgram(ShaderTable::depth_shader_info.name.c_str(),
                                                                                            ShaderTable::depth_shader_info.vert_path.c_str(),
                                                                                            ShaderTable::depth_shader_info.frag_path.c_str())});
        shader_program_map.insert({ShaderTable::skybox_shader_info.name, new GLShaderProgram(ShaderTable::skybox_shader_info.name.c_str(),
                                                                                             ShaderTable::skybox_shader_info.vert_path.c_str(),
                                                                                             ShaderTable::skybox_shader_info.frag_path.c_str())});
        shader_program_map.insert({ShaderTable::pure_shader_info.name, new GLShaderProgram(ShaderTable::pure_shader_info.name.c_str(),
                                                                                           ShaderTable::pure_shader_info.vert_path.c_str(),
                                                                                           ShaderTable::pure_shader_info.frag_path.c_str())});
        shader_program_map.insert({ShaderTable::blinn_phong_info.name, new GLShaderProgram(ShaderTable::blinn_phong_info.name.c_str(),
                                                                                           ShaderTable::blinn_phong_info.vert_path.c_str(),
                                                                                           ShaderTable::blinn_phong_info.frag_path.c_str())});
        shader_program_map.insert({ShaderTable::simple_depth_shader_info.name,new GLShaderProgram(ShaderTable::simple_depth_shader_info.name.c_str(),
                                                                                             ShaderTable::simple_depth_shader_info.vert_path.c_str(),
                                                                                             ShaderTable::simple_depth_shader_info.frag_path.c_str())});

    }

    void DawnEngine::InitMaterials() {
        this->material_map.insert({"default", DawnMaterial()});
        this->material_map.insert({"depth",DawnMaterial(ShaderTable::depth_shader_info)});
    }

    void DawnEngine::EnableGLFeatures() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);



    }

    void DawnEngine::MountUISystem(DawnUISystem *ui_system) {
        this->ui_system_ = ui_system;
        this->ui_system_->initialize(this->render_window_->getWindowPtr());

    }

    std::vector<GameObject *> DawnEngine::GetGameObjectPtrs() const {
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

    void DawnEngine::SetUniformInShaderPrograms(const std::vector<std::string>& shader_program_names, const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) {
        for (const auto &name: shader_program_names) {
            auto shaderProgramPtr = this->shader_program_map.at(name);
            if (shaderProgramPtr != nullptr) {
                shaderProgramPtr->Activate();
                shaderProgramPtr->SetUniforms(uniforms);
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


    RayHitInfo DawnEngine::RayCastDetection(Ray ray) {
        std::map<float, RayHitInfo> hit_info_queue;
        for (auto game_obj: this->game_object_ptrs) {
            auto collider_m = game_obj->GetModule<dawn_engine::ColliderModule>();
            if (collider_m != nullptr && collider_m->GetActivation()) {
                auto transform_m = game_obj->GetModule<TransformModule>();
                auto inv_mat = glm::inverse(transform_m->GetTranslationMat() * transform_m->GetRotationMat());
                auto transformed_ray = TransformRay(inv_mat, ray);
                auto hit_info = CollisionDetection(transformed_ray,
                                                   collider_m->GetColliderShape(),
                                                   game_obj->GetModule<TransformModule>()->GetScaleMat());
                if (hit_info.any_hit) {
                    hit_info_queue.insert({hit_info.distance, hit_info});
                }
            }
        }
        return hit_info_queue.begin()->second;
    }



    GameObject *DawnEngine::FindGameObjectByName(std::string name) {
        for(auto game_obj : this->game_object_ptrs){
            if(game_obj->GetName() == name){
                return game_obj;
            }
        }
        return nullptr;
    }

    GameObject *DawnEngine::CreatePrimitive(PrimitiveType pType) {
        switch (pType) {
            case BoxPrimitive: {
                std::vector<float> positionRaw = {
                        -0.5f, -0.5f, -0.5f, // face 1: left
                        -0.5f, -0.5f, 0.5f,
                        -0.5f, 0.5f, -0.5f,
                        -0.5f, 0.5f, 0.5f, // indices_: {0 1 2} {1 3 2}
                        0.5f, -0.5f, -0.5f, // face 2: right
                        0.5f, -0.5f, 0.5f,
                        0.5f, 0.5f, -0.5f,
                        0.5f, 0.5f, 0.5f, // indices_: {4 6 5} {5 6 7}
                        -0.5f, -0.5f, -0.5f, // face 3: bottom
                        -0.5f, -0.5f, 0.5f,
                        0.5f, -0.5f, -0.5f,
                        0.5f, -0.5f, 0.5f, // indices_: {8 10 9} {9 10 11}
                        -0.5f, 0.5f, -0.5f, // face 4: top
                        -0.5f, 0.5f, 0.5f,
                        0.5f, 0.5f, -0.5f,
                        0.5f, 0.5f, 0.5f, // indices_: {12 13 14} {13 15 14}
                        -0.5f, -0.5f, 0.5f, // face 5: front
                        -0.5f, 0.5f, 0.5f,
                        0.5f, -0.5f, 0.5f,
                        0.5f, 0.5f, 0.5f, // indices_: {16 18 17} {17 18 19}
                        -0.5f, -0.5f, -0.5f, // face 6: back
                        -0.5f, 0.5f, -0.5f,
                        0.5f, -0.5f, -0.5f,
                        0.5f, 0.5f, -0.5f, // indices_: {20 21 22} {21 23 22}
                };
                std::vector<float> normalRaw = {-1.0f, 0.0f, 0.0f,
                                                -1.0f, 0.0f, 0.0f,
                                                -1.0f, 0.0f, 0.0f,
                                                -1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                };
                std::vector<float> texCoordsRaw = {0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f};
                int num_vertices = 24;  // 6 faces * 4 vertex on each face (2 triangles on each face)
                std::vector<DawnVertex> vertices;
                for (int i = 0; i < num_vertices; ++i) {
                    DawnVertex tmpV = {};
                    tmpV.position = glm::vec3(positionRaw[i * 3], positionRaw[i * 3 + 1], positionRaw[i * 3 + 2]);
                    tmpV.normal = glm::vec3(normalRaw[i * 3], normalRaw[i * 3 + 1], normalRaw[i * 3 + 2]);
                    tmpV.texCoords = glm::vec2(texCoordsRaw[i * 2], texCoordsRaw[i * 2 + 1]);
                    vertices.push_back(tmpV);
                }
                std::vector<unsigned int> indices = {0, 1, 2, 1, 3, 2,
                                                     4, 6, 5, 5, 6, 7,
                                                     8, 10, 9, 9, 10, 11,
                                                     12, 13, 14, 13, 15, 14,
                                                     16, 18, 17, 17, 18, 19,
                                                     20, 21, 22, 21, 23, 22
                };
                DawnMesh mesh = DawnMesh(vertices, indices, std::make_shared<DawnMaterial>(this->material_map.at("default")));
                auto *cubePrimitive(new GameObject("New Cube Primitive", true));
                DawnModel model({mesh});
                cubePrimitive->AddModule<RendererModule>(model);
                return cubePrimitive;
                break;
            }
            default:
                break;
        }
        return nullptr;
    }

    void DawnEngine::AddMaterial(const std::string & name, const DawnMaterial& material) {
        this->material_map.insert({name,material});

    }

    void DawnEngine::AddShaderProgram(const std::string &name, GLShaderProgram *shader_program) {
        this->shader_program_map.insert({name, shader_program});
    }

    GLShaderProgram *DawnEngine::GetShaderProgram(const std::string &name) {
        if(this->shader_program_map.count(name) == 0){
            throw std::runtime_error(fmt::format("shader program: {} not exists in table",name));
        }
        return this->shader_program_map.at(name);
    }

    DawnMaterial DawnEngine::GetMaterial(const std::string &name) {
        if(this->material_map.count(name) == 0){
            throw std::runtime_error(fmt::format("material : {} not exists in table",name));
        }
        return this->material_map.at(name);
    }


} // namespace dawn_engine