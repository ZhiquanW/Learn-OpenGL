//
// Created by zhiquan on 5/28/22.
//

#include "helloworld_ui_system.h"

namespace helloworld {

    HelloWorldUISystem::HelloWorldUISystem(int glslVersion) : dawn_engine::DawnUISystem(glslVersion) {
    }

    void HelloWorldUISystem::start(dawn_engine::DawnEngine *enginePtr) {
        this->InitGameObjectTracker(enginePtr->GetGameObjectPtrs());
        this->InitGameObjectMonitors(enginePtr->GetGameObjectPtrs());
        this->InitGlobalSettingsPanel(enginePtr);
        this->InitPrefabManager();
    }

    void HelloWorldUISystem::update(dawn_engine::DawnEngine *enginePtr) {
        this->OnMouseClicked();
        this->UpdateGlobalSettingsPanel(enginePtr);
        this->UpdateGameObjectTracker(enginePtr->GetGameObjectPtrs());
        this->UpdateGameObjectMonitors(enginePtr->GetGameObjectPtrs());
        this->UpdatePrefabManager(enginePtr->GetPrefabPaths());
        this->UpdateCreationPopup(enginePtr);
    }

    void HelloWorldUISystem::InitGlobalSettingsPanel(dawn_engine::DawnEngine *enginePtr) {
        this->selected_shader_str = enginePtr->GetShaderProgramMapMeta().begin()->first;
    }

    void HelloWorldUISystem::UpdateGlobalSettingsPanel(dawn_engine::DawnEngine *engine_ptr) {
        ImGui::Begin("Global Settings");
        // shader program selection
        if (ImGui::CollapsingHeader("Shaders", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Button("Reload")) {
                engine_ptr->GetShaderProgramMapMeta().at(this->selected_shader_str)->Reload();
            }
            ImGui::SameLine();
            if (ImGui::BeginCombo("Shader Selector", this->selected_shader_str.c_str())) {
                for (const auto &mapPair: engine_ptr->GetShaderProgramMapMeta()) {
                    bool is_selected = selected_shader_str.c_str() == mapPair.first.c_str();
                    if (ImGui::Selectable(mapPair.first.c_str(), is_selected)) {
                        selected_shader_str = mapPair.first;
                    }
                }
                ImGui::EndCombo();
            }

        }
        // main camera properties
        if (ImGui::CollapsingHeader("Main Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            dawn_engine::Camera &camMeta = engine_ptr->GetMainCameraRef();
            ImGui::DragFloat("move speed", &camMeta.getMoveSpeedMeta(), this->defaultDragSpeed, 0.0f,
                             this->defaultMaxValue);
            ImGui::DragFloat("sensitivity", &camMeta.getMouseSensitivityMeta(), this->defaultDragSpeed, 0.0f, 1.0f);
            ImGui::DragFloat("field of view", &camMeta.getFovMeta(), this->defaultDragSpeed, 0.0f,
                             this->defaultMaxValue);
            ImGui::DragFloat("zNear", &engine_ptr->GetMainCameraRef().getZNearMeta(), this->defaultDragSpeed, 0.001f,
                             engine_ptr->GetMainCameraRef().GetZFar());
            ImGui::DragFloat("zFar", &engine_ptr->GetMainCameraRef().getZFarMeta(), this->defaultDragSpeed,
                             engine_ptr->GetMainCameraRef().getZNearMeta(),
                             this->defaultMaxValue);
        }
        ImGui::End();

    }


    void HelloWorldUISystem::InitGameObjectTracker(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs) {
        this->gameObjectTrackerSelectionTable.resize(gameObjectPtrs.size());
    }

    void HelloWorldUISystem::UpdateGameObjectTracker(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs) {
        if (gameObjectPtrs.size() > this->gameObjectTrackerSelectionTable.size()) {
            this->gameObjectTrackerSelectionTable.resize(gameObjectPtrs.size());
        }
        ImGui::ShowDemoWindow();
        ImGui::Begin("Game Object Tracker");
        uint32_t selection_idx = 0;
        for (auto gObj: gameObjectPtrs) {
            std::string indexed_name = fmt::format("{}: {}", selection_idx,
                                                   gObj->GetName().c_str());
            if (ImGui::Selectable(indexed_name.c_str(),
                                  (bool) this->gameObjectTrackerSelectionTable[selection_idx],
                                  ImGuiSelectableFlags_AllowDoubleClick)) {
//                if (ImGui::IsMouseDoubleClicked(0)) {
//                }
                if (!ImGui::GetIO().KeyCtrl) { // Clear gameObjectTrackerSelectionTable when CTRL is not held
                    memset(this->gameObjectTrackerSelectionTable.data(), 0,
                           sizeof(int) * this->gameObjectTrackerSelectionTable.size());
//                    for (auto v: this->gameObjectTrackerSelectionTable) {
//                    }
                }
                this->gameObjectTrackerSelectionTable[selection_idx] ^= 1;
            }
            selection_idx += 1;
        }
        ImGui::End();

    }

    void HelloWorldUISystem::InitGameObjectMonitors(const std::vector<dawn_engine::GameObject *> &game_obj_ptrs) {}

    void HelloWorldUISystem::UpdateGameObjectMonitors(const std::vector<dawn_engine::GameObject *> &game_objs) {
        uint32_t gameObjectNum = game_objs.size();
        for (int i = 0; i < gameObjectNum; ++i) {
            if (this->gameObjectTrackerSelectionTable[i]) {
                std::string indexed_name = fmt::format("{}: {}", i, game_objs[i]->GetName().c_str());
                ImGui::Begin(indexed_name.c_str());
                for (const auto &modulesPair: game_objs[i]->GetAllModules()) {
                    for (const auto &module_ptr: modulesPair.second) {
                        this->EmbedModuleMonitor(module_ptr);
                    }
                }
                ImGui::End();
            }
        }
    }

    void HelloWorldUISystem::UpdateBehaviourModule(dawn_engine::BehaviourModule *behaviour_module) {
        if (ImGui::CollapsingHeader(behaviour_module->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("module activation", &behaviour_module->GetActivationMeta());
            behaviour_module->UpdateModuleMonitor();
        }
    }

    void HelloWorldUISystem::UpdateTransformModuleMonitor(dawn_engine::TransformModule *const transformModule) const {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat3("position", &transformModule->getPositionMeta()[0], this->defaultDragSpeed,
                              this->defaultMinValue, this->defaultMaxValue);
            ImGui::DragFloat3("rotation", &transformModule->getRotationMeta()[0], this->defaultDragSpeed,
                              this->defaultMinValue, this->defaultMaxValue);
            ImGui::DragFloat3("scale", &transformModule->getScaleMeta()[0], this->defaultDragSpeed,
                              this->defaultMinValue, this->defaultMaxValue);
        }

    }

    void HelloWorldUISystem::UpdateMeshModuleMonitor(dawn_engine::RendererModule *meshModule) {
        if (ImGui::CollapsingHeader("mesh module", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("module activation", &meshModule->GetActivationMeta());
            ImGui::Text("mesh num: %zu", meshModule->getMeshes().size());
            std::string label = fmt::format("mesh activation##{}", 0);
            int meshNum = (int) meshModule->getMeshes().size();
            for (int meshIdx = 0; meshIdx < meshNum; ++meshIdx) {
                dawn_engine::DawnMesh &tmpMesh = meshModule->getMeshesRef()[meshIdx];
                bool enabledLightingMaps = tmpMesh.EnableLightingMap();
                std::string treeNodeLabel = fmt::format("mesh node: {}", meshIdx);
                if (ImGui::TreeNode(treeNodeLabel.c_str())) {
                    ImGui::Text("vertex num: %d", meshModule->getMeshesRef()[meshIdx].GetVerticesNum());
                    std::string materialType = "pure color";
                    if (enabledLightingMaps) {
                        materialType = "lighting map";
                    }
                    std::string materialModeStr = "material mode: " + materialType;
                    ImGui::Text("%s", materialModeStr.c_str());
                    std::string CheckboxLabel = fmt::format("mesh activation##{}", meshIdx);
                    ImGui::Checkbox(CheckboxLabel.c_str(), &meshModule->getActivationsRef()[meshIdx]);
                    selectedTransparencyTypeStr = meshOpaqueOptions[int(!tmpMesh.GetMaterialPtr()->GetOpaque())];
                    if (ImGui::BeginCombo("Transparency Type", selectedTransparencyTypeStr.c_str())) {
                        for (int i = 0; i < 2; ++i) {
                            bool isSelected = selectedTransparencyTypeStr.c_str() == meshOpaqueOptions[i].c_str();
                            if (ImGui::Selectable(meshOpaqueOptions[i].c_str(), isSelected)) {
                                selectedTransparencyTypeStr = meshOpaqueOptions[i];
                                tmpMesh.GetMaterialPtr()->SetOpaque(!bool(i));
                            }
                        }
                        ImGui::EndCombo();
                    }
                    if (!tmpMesh.GetMaterialPtr()->GetOpaque()) {
                        ImGui::DragFloat("transparency", &tmpMesh.GetMaterialPtr()->getTransparencyRef(),
                                         this->defaultDragSpeed, 0.0f, 1.0f);
                    }
                    ImGui::ColorEdit3("ambient", &tmpMesh.GetMaterialPtr()->GetAmbientColorRef()[0]);
                    if (!enabledLightingMaps) {
                        ImGui::ColorEdit3("diffuse", &tmpMesh.GetMaterialPtr()->GetDiffuseColorRef()[0]);
                        ImGui::ColorEdit3("specular", &tmpMesh.GetMaterialPtr()->GetSpecularColorRef()[0]);
                    }
                    ImGui::DragFloat("shininess", &tmpMesh.GetMaterialPtr()->GetShininessRef(), this->defaultDragSpeed,
                                     0.0f, this->defaultMaxValue);
                    ImGui::TreePop();
                }
            }

//            ImGui::ColorEdit3("diffuse", &meshModule->GetMaterialRef().GetDiffuseMeta()[0]);
//            ImGui::ColorEdit3("specular", &meshModule->GetMaterialRef().GetDiffuseMeta()[0]);
        }
    }

    void HelloWorldUISystem::EmbedBaseLightMonitor(dawn_engine::LightModule *lightModule) const {
        ImGui::ColorEdit3("ambient", &lightModule->GetAmbientMeta()[0]);
        ImGui::ColorEdit3("diffuse", &lightModule->GetDiffuseMeta()[0]);
        ImGui::ColorEdit3("specular", &lightModule->GetSpecularMeta()[0]);

    }

    void
    HelloWorldUISystem::UpdatePointLightModuleMonitor(dawn_engine::PointLightModule *const pointLightModule) const {
        if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("activation", &pointLightModule->GetActivationMeta());
            this->EmbedBaseLightMonitor(pointLightModule);
            ImGui::Separator();
            ImGui::DragFloat("constant", &pointLightModule->constant, this->defaultDragSpeed, 0.0f,
                             this->defaultMaxValue);
            ImGui::DragFloat("linear", &pointLightModule->linear, this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
            ImGui::DragFloat("quadratic", &pointLightModule->quadratic, this->defaultDragSpeed, 0.0f,
                             this->defaultMaxValue);
        }
    }

    void HelloWorldUISystem::UpdateDirectionLightModuleMonitor(
            dawn_engine::DirectionalLightModule *const directionalLightModule) const {
        if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("activation", &directionalLightModule->GetActivationMeta());
            this->EmbedBaseLightMonitor(directionalLightModule);
            ImGui::Separator();
            ImGui::DragFloat3("direction", &directionalLightModule->GetDirectionRef()[0], this->defaultDragSpeed,
                              this->defaultMinValue, this->defaultMaxValue);
            ImGui::DragFloat4("box range", directionalLightModule->GetBoxRangeRef(), this->defaultDragSpeed, this->defaultMinValue,
                             this->defaultMaxValue);
            ImGui::DragFloat2("z range", directionalLightModule->GetZRangeRef(), this->defaultDragSpeed, 0.0f,
                              this->defaultMaxValue);
        }
    }

    void HelloWorldUISystem::UpdateSpotLightModuleMonitor(dawn_engine::SpotLightModule *const spotLightModule) const {
        if (ImGui::CollapsingHeader("Spot Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("activation", &spotLightModule->GetActivationMeta());
            this->EmbedBaseLightMonitor(spotLightModule);
            ImGui::Separator();
            ImGui::DragFloat("constant", &spotLightModule->getQuadraticMeta(), this->defaultDragSpeed, 0.0f,
                             this->defaultMaxValue);
            ImGui::DragFloat("linear", &spotLightModule->getLinearMeta(), this->defaultDragSpeed, 0.0f,
                             this->defaultMaxValue);
            ImGui::DragFloat("quadratic", &spotLightModule->getQuadraticMeta(), this->defaultDragSpeed, 0.0f,
                             this->defaultMaxValue);
            ImGui::DragFloat3("direction", &spotLightModule->getDirectionMeta()[0], this->defaultDragSpeed,
                              this->defaultMinValue, this->defaultMaxValue);
            ImGui::DragFloat("inner range", &spotLightModule->getInnerRangeMeta(), this->defaultDragSpeed, 0.0f,
                             spotLightModule->getOuterRange());
            ImGui::DragFloat("outer range", &spotLightModule->getOuterRangeMeta(), this->defaultDragSpeed,
                             spotLightModule->getInnerRange(), 180.0f);
        }
    }

    void HelloWorldUISystem::UpdateColliderModuleMonitor(dawn_engine::ColliderModule *const collider_module) const {
        if (ImGui::CollapsingHeader("Collider Module", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("activation", &collider_module->GetActivationMeta());
        }
    }

    void HelloWorldUISystem::EmbedModuleMonitor(dawn_engine::BaseModule *targetModule) {

        if (dynamic_cast<dawn_engine::TransformModule *>(targetModule) != nullptr) {
            this->UpdateTransformModuleMonitor(dynamic_cast<dawn_engine::TransformModule *>(targetModule));
        } else if (dynamic_cast<dawn_engine::SpotLightModule *>(targetModule) != nullptr) {
            this->UpdateSpotLightModuleMonitor(dynamic_cast<dawn_engine::SpotLightModule *>(targetModule));
        } else if (dynamic_cast<dawn_engine::PointLightModule * > (targetModule) != nullptr) {
            this->UpdatePointLightModuleMonitor(dynamic_cast<dawn_engine::PointLightModule * > (targetModule));
        } else if (dynamic_cast<dawn_engine::DirectionalLightModule *>(targetModule) != nullptr) {
            this->UpdateDirectionLightModuleMonitor(dynamic_cast<dawn_engine::DirectionalLightModule *>(targetModule));
        } else if (dynamic_cast<dawn_engine::RendererModule *> (targetModule)) {
            this->UpdateMeshModuleMonitor(dynamic_cast<dawn_engine::RendererModule *> (targetModule));
        } else if (dynamic_cast<dawn_engine::ColliderModule *>(targetModule)) {
            this->UpdateColliderModuleMonitor(dynamic_cast<dawn_engine::ColliderModule *> (targetModule));
        } else if (dynamic_cast<dawn_engine::BehaviourModule *> (targetModule)) {
            this->UpdateBehaviourModule(dynamic_cast<dawn_engine::BehaviourModule *> (targetModule));
        } else {
            throw std::runtime_error("Unknown Module Type");
        }
    }


    void HelloWorldUISystem::UpdateCreationPopup(dawn_engine::DawnEngine *engine_ptr) {
        if (ImGui::BeginPopup("CreationPopup")) {
            if (ImGui::Button("Box Link")) {
                auto box_block = engine_ptr->CreatePrimitive(dawn_engine::BoxPrimitive);
                box_block->AddModule<BoxBlock>(box_block->GetModule<TransformModule>()->GetScale());
//                engine_ptr->AddGameObject(engine_ptr->CreatePrimitive(dawn_engine::BoxPrimitive));

                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    Ray
    HelloWorldUISystem::GenRayOnClick(glm::vec3 start_pos, glm::vec3 cam_front, glm::vec3 cam_up, glm::vec3 cam_right,
                                      float fov, glm::vec2 win_size, glm::vec2 mouse_pos) {
        glm::vec2 win_center = win_size / 2.0f;
        float plane_z = win_center.y / glm::tan(glm::radians(fov) / 2);
        glm::vec2 offset2center = mouse_pos - win_center;
        offset2center.y *= -1;
        glm::vec3 x_axis = cam_right * offset2center.x;
        glm::vec3 y_axis = cam_up * offset2center.y;
        glm::vec3 z_axis = cam_front * plane_z;
        glm::vec3 dir = glm::normalize(x_axis + y_axis + z_axis);
        return {start_pos, dir};
    }

    void HelloWorldUISystem::OnMouseClicked() {
        ImGuiIO &io = ImGui::GetIO();
        if (!io.WantCaptureMouse && ImGui::IsMouseClicked(0)) {
            // draw ray on click
            glm::vec2 win_size = glm::vec2(io.DisplaySize.x, io.DisplaySize.y);
            auto camera = dawn_engine::DawnEngine::instance->GetMainCameraRef();
            Ray ray = GenRayOnClick(camera.GetPosition(),
                                    camera.GetFront(),
                                    camera.GetUp(),
                                    camera.GetRight(),
                                    camera.GetFov(),
                                    win_size,
                                    glm::vec2(io.MousePos.x, io.MousePos.y));
//            auto *game_obj = new dawn_engine::GameObject("Ray Line", true);
//            std::vector<dawn_engine::DawnVertex> vertices = {dawn_engine::DawnVertex(ray.GetOrigin()),
//                                                             dawn_engine::DawnVertex(ray.ToDirection(100.0f))};
//            auto line_material = std::make_shared<dawn_engine::DawnMaterial>(glm::vec3(1.0f, 1.0f, 0.0f));
//            auto line_mesh = dawn_engine::DawnMesh(vertices, {0, 1}, line_material);
//            game_obj->AddModule<dawn_engine::RendererModule>(dawn_engine::DawnModel({line_mesh}));
//            dawn_engine::DawnEngine::instance->AddGameObject(game_obj);
//            // collision detection on click
            auto hit_info = dawn_engine::DawnEngine::instance->RayCastDetection(ray);
            if (hit_info.any_hit) {
                hit_info.game_object_ptr->GetModule<BoxBlock>()->AddAttachPoint(hit_info.local_pos,
                                                                                glm::vec3(hit_info.local_normal));

            } else {
                std::cout << " no hit" << std::endl;
            }

        }
        if (ImGui::IsMouseClicked(1)) {
            ImGui::OpenPopup("CreationPopup");

        }
    }

    void HelloWorldUISystem::UpdatePrefabManager(const std::set<std::string> &prefab_paths) {
        if (prefab_paths.size() > this->prefab_selection_table.size()) {
            this->prefab_selection_table.resize(prefab_paths.size());
        }
        ImGui::Begin("Prefab Manager");
        uint32_t selection_idx = 0;
        for (auto prefab_name:prefab_paths) {
            std::string indexed_name = fmt::format("{}: {}", selection_idx, prefab_name.c_str());
            if (ImGui::Selectable(indexed_name.c_str(),
                                  (bool) this->prefab_selection_table[selection_idx],
                                  ImGuiSelectableFlags_AllowDoubleClick)) {
                if (ImGui::IsMouseDoubleClicked(0)) {
                    DawnEngine::instance->LoadPrefab(prefab_name);
                }
                memset(this->gameObjectTrackerSelectionTable.data(), 0,
                       sizeof(int) * this->gameObjectTrackerSelectionTable.size());
                this->prefab_selection_table[selection_idx] ^= 1;
            }

            selection_idx += 1;
        }
        ImGui::End();

    }

    void HelloWorldUISystem::InitPrefabManager() {
        namespace fs = std::filesystem;
        const fs::path target_path{"../prefabs"};
        for (const auto &entry : fs::directory_iterator(target_path)) {
            const auto file_name = entry.path().filename().string();
            if (entry.is_regular_file()) {
                size_t lastindex = file_name.find_last_of(".");
                std::string raw_name = file_name.substr(0, lastindex);
                DawnEngine::instance->AddPrefabName(raw_name);
            }
        }

    }


}