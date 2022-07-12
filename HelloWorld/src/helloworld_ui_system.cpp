//
// Created by zhiquan on 5/28/22.
//

#include "helloworld_ui_system.h"

namespace helloworld {

    HelloWorldUISystem::HelloWorldUISystem(int glslVersion) : dawn_engine::DawnUISystem(glslVersion) {
    }

    void HelloWorldUISystem::start(dawn_engine::DawnEngine *enginePtr) {
        this->initGameObjectTracker(enginePtr->getGameObjectPtrs());
        this->initGameObjectMonitors(enginePtr->getGameObjectPtrs());
        this->initGlobalSettingsPanel(enginePtr);
    }

    void HelloWorldUISystem::update(dawn_engine::DawnEngine *enginePtr) {
        this->OnMouseClicked();
        this->updateGlobalSettingsPanel(enginePtr);
        this->updateGameObjectTracker(enginePtr->getGameObjectPtrs());
        this->updateGameObjectMonitors(enginePtr->getGameObjectPtrs());
        this->UpdateCreationPopup(enginePtr);
    }

    void HelloWorldUISystem::initGlobalSettingsPanel(dawn_engine::DawnEngine *enginePtr) {
        this->selected_shader_str = enginePtr->GetShaderProgramMapMeta().begin()->first;
    }

    void HelloWorldUISystem::updateGlobalSettingsPanel(dawn_engine::DawnEngine *engine_ptr) {
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
            ImGui::DragFloat("move speed", &camMeta.getMoveSpeedMeta(), this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
            ImGui::DragFloat("sensitivity", &camMeta.getMouseSensitivityMeta(), this->defaultDragSpeed, 0.0f, 1.0f);
            ImGui::DragFloat("field of view", &camMeta.getFovMeta(), this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
            ImGui::DragFloat("zNear", &engine_ptr->GetMainCameraRef().getZNearMeta(), this->defaultDragSpeed, 0.001f, engine_ptr->GetMainCameraRef().GetZFar());
            ImGui::DragFloat("zFar", &engine_ptr->GetMainCameraRef().getZFarMeta(), this->defaultDragSpeed, engine_ptr->GetMainCameraRef().getZNearMeta(),
                             this->defaultMaxValue);
        }
        ImGui::End();

    }

    void HelloWorldUISystem::initGameObjectTracker(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs) {
        this->gameObjectTrackerSelectionTable.resize(gameObjectPtrs.size());
    }

    void HelloWorldUISystem::updateGameObjectTracker(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs) {
        ImGui::ShowDemoWindow();
        ImGui::Begin("Game Object Tracker");
        uint32_t selection_idx = 0;
        for (auto gObj: gameObjectPtrs) {
            std::string indexed_name = fmt::format("{}: {}", selection_idx,
                                                   gObj->getName().c_str());
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

    void HelloWorldUISystem::initGameObjectMonitors(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs) {}

    void HelloWorldUISystem::updateGameObjectMonitors(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs) {
        uint32_t gameObjectNum = gameObjectPtrs.size();
        for (int i = 0; i < gameObjectNum; ++i) {
            if (this->gameObjectTrackerSelectionTable[i]) {
                std::string indexed_name = fmt::format("{}: {}", i, gameObjectPtrs[i]->getName().c_str());
                ImGui::Begin(indexed_name.c_str());
                for (const auto &modulesPair: gameObjectPtrs[i]->getModules()) {
                    for (const auto &modulePtr: modulesPair.second) {
                        this->embedModuleMonitor(modulePtr);
                    }
                }
                ImGui::End();
            }
        }
    }


    void HelloWorldUISystem::updateTransformModuleMonitor(dawn_engine::TransformModule *const transformModule) const {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat3("position", &transformModule->getPositionMeta()[0], this->defaultDragSpeed, this->defaultMinValue, this->defaultMaxValue);
            ImGui::DragFloat3("rotation", &transformModule->getRotationMeta()[0], this->defaultDragSpeed, this->defaultMinValue, this->defaultMaxValue);
            ImGui::DragFloat3("scale", &transformModule->getScaleMeta()[0], this->defaultDragSpeed, this->defaultMinValue, this->defaultMaxValue);
        }

    }

    void HelloWorldUISystem::updateMeshModuleMonitor(dawn_engine::RendererModule *meshModule) {
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
                    selectedTransparencyTypeStr = meshOpaqueOptions[int(!tmpMesh.GetMaterialRef().GetOpaque())];
                    if (ImGui::BeginCombo("Transparency Type", selectedTransparencyTypeStr.c_str())) {
                        for (int i = 0; i < 2; ++i) {
                            bool isSelected = selectedTransparencyTypeStr.c_str() == meshOpaqueOptions[i].c_str();
                            if (ImGui::Selectable(meshOpaqueOptions[i].c_str(), isSelected)) {
                                selectedTransparencyTypeStr = meshOpaqueOptions[i];
                                tmpMesh.GetMaterialRef().SetOpaque(!bool(i));
                            }
                        }
                        ImGui::EndCombo();
                    }
                    if (!tmpMesh.GetMaterialRef().GetOpaque()) {
                        ImGui::DragFloat("transparency", &tmpMesh.GetMaterialRef().getTransparencyRef(), this->defaultDragSpeed, 0.0f, 1.0f);
                    }
                    ImGui::ColorEdit3("ambient", &tmpMesh.GetMaterialRef().GetAmbientColorRef()[0]);
                    if (!enabledLightingMaps) {
                        ImGui::ColorEdit3("diffuse", &tmpMesh.GetMaterialRef().GetDiffuseColorRef()[0]);
                        ImGui::ColorEdit3("specular", &tmpMesh.GetMaterialRef().GetSpecularColorRef()[0]);
                    }
                    ImGui::DragFloat("shininess", &tmpMesh.GetMaterialRef().GetShininessRef(), this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
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

    void HelloWorldUISystem::updatePointLightModuleMonitor(dawn_engine::PointLightModule *const pointLightModule) const {
        if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("activation", &pointLightModule->GetActivationMeta());
            this->EmbedBaseLightMonitor(pointLightModule);
            ImGui::Separator();
            ImGui::DragFloat("constant", &pointLightModule->constant, this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
            ImGui::DragFloat("linear", &pointLightModule->linear, this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
            ImGui::DragFloat("quadratic", &pointLightModule->quadratic, this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
        }
    }


    void HelloWorldUISystem::updateDirectionLightModuleMonitor(dawn_engine::DirectionalLightModule *const directionalLightModule) const {
        if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("activation", &directionalLightModule->GetActivationMeta());
            this->EmbedBaseLightMonitor(directionalLightModule);
            ImGui::Separator();
            ImGui::DragFloat3("direction", &directionalLightModule->getDirectionMeta()[0], this->defaultDragSpeed, this->defaultMinValue, this->defaultMaxValue);
        }
    }

    void HelloWorldUISystem::updateSpotLightModuleMonitor(dawn_engine::SpotLightModule *const spotLightModule) const {
        if (ImGui::CollapsingHeader("Spot Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("activation", &spotLightModule->GetActivationMeta());
            this->EmbedBaseLightMonitor(spotLightModule);
            ImGui::Separator();
            ImGui::DragFloat("constant", &spotLightModule->getQuadraticMeta(), this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
            ImGui::DragFloat("linear", &spotLightModule->getLinearMeta(), this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
            ImGui::DragFloat("quadratic", &spotLightModule->getQuadraticMeta(), this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
            ImGui::DragFloat3("direction", &spotLightModule->getDirectionMeta()[0], this->defaultDragSpeed, this->defaultMinValue, this->defaultMaxValue);
            ImGui::DragFloat("inner range", &spotLightModule->getInnerRangeMeta(), this->defaultDragSpeed, 0.0f, spotLightModule->getOuterRange());
            ImGui::DragFloat("outer range", &spotLightModule->getOuterRangeMeta(), this->defaultDragSpeed, spotLightModule->getInnerRange(), 180.0f);
        }
    }

    void HelloWorldUISystem::embedModuleMonitor(dawn_engine::BaseModule *const targetModule) {

        if (dynamic_cast<dawn_engine::TransformModule *>(targetModule) != nullptr) {
            this->updateTransformModuleMonitor(dynamic_cast<dawn_engine::TransformModule *>(targetModule));
        } else if (dynamic_cast<dawn_engine::SpotLightModule *>(targetModule) != nullptr) {
            this->updateSpotLightModuleMonitor(dynamic_cast<dawn_engine::SpotLightModule *>(targetModule));
        } else if (dynamic_cast<dawn_engine::PointLightModule * > (targetModule) != nullptr) {
            this->updatePointLightModuleMonitor(dynamic_cast<dawn_engine::PointLightModule * > (targetModule));
        } else if (dynamic_cast<dawn_engine::DirectionalLightModule *>(targetModule) != nullptr) {
            this->updateDirectionLightModuleMonitor(dynamic_cast<dawn_engine::DirectionalLightModule *>(targetModule));
        } else if (dynamic_cast<dawn_engine::RendererModule *> (targetModule)) {
            this->updateMeshModuleMonitor(dynamic_cast<dawn_engine::RendererModule *> (targetModule));
        } else {
            std::cout << "unknown module" << std::endl;
        }
//        ImGui::End();
    }

    void HelloWorldUISystem::InitCreationPopup(dawn_engine::DawnEngine *engine_ptr) {

    }

    void HelloWorldUISystem::UpdateCreationPopup(dawn_engine::DawnEngine *engine_ptr) {
        if (ImGui::BeginPopup("CreationPopup")) {
            if (ImGui::Button("Cube")) {
                engine_ptr->AddGameObject(dawn_engine::GameObject::CreatePrimitive(dawn_engine::BoxPrimitive));
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    dawn_engine::Ray HelloWorldUISystem::GenRayOnClick(glm::vec3 start_pos, glm::vec3 cam_dir, float fov, glm::vec2 win_size, glm::vec2 mouse_pos) {
        glm::vec2 win_center = win_size / 2.0f;
        float plane_z = win_center.y / glm::tan(glm::radians(fov / 2.0f));
        glm::vec2 offset2center = mouse_pos - win_center;
        offset2center.y *= -1;
        glm::vec3 local_dir = glm::vec3(offset2center, plane_z) / plane_z;
        return {start_pos, cam_dir + glm::vec3(local_dir.x, local_dir.y, 0.0f)};
    }

    void HelloWorldUISystem::OnMouseClicked() {
        if (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            ImGuiIO &io = ImGui::GetIO();
            glm::vec2 win_size = glm::vec2(io.DisplaySize.x, io.DisplaySize.y);
            dawn_engine::Ray ray = GenRayOnClick(dawn_engine::DawnEngine::instance->GetMainCameraRef().GetPosition(),
                                                 dawn_engine::DawnEngine::instance->GetMainCameraRef().GetFront(),
                                                 dawn_engine::DawnEngine::instance->GetMainCameraRef().GetFov(),
                                                 win_size,
                                                 glm::vec2(io.MousePos.x, io.MousePos.y));
            auto *game_obj = new dawn_engine::GameObject("Ray Line", true);
//            std::vector<dawn_engine::DawnVertex> vertices = {dawn_engine::DawnVertex(ray.GetStartPos()), dawn_engine::DawnVertex(ray.ToDirection(100))};
            std::vector<dawn_engine::DawnVertex> vertices = {dawn_engine::DawnVertex(ray.GetStartPos()), dawn_engine::DawnVertex(ray.ToDirection(100.0f))};
            auto line_mesh = dawn_engine::DawnMesh(vertices, {0, 1}, dawn_engine::DawnMaterial(glm::vec3(1.0f, 1.0f, 0.0f)));
            game_obj->AddModule<dawn_engine::RendererModule>(dawn_engine::DawnModel({line_mesh}));
            dawn_engine::DawnEngine::instance->AddGameObject(game_obj);

        }
        if (ImGui::IsMouseClicked(1)) {
            ImGui::OpenPopup("CreationPopup");

        }
    }


}