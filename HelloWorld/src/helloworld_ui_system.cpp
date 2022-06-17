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
        this->updateGlobalSettingsPanel(enginePtr);
        this->updateGameObjectTracker(enginePtr->getGameObjectPtrs());
        this->updateGameObjectMonitors(enginePtr->getGameObjectPtrs());
    }

    void HelloWorldUISystem::initGlobalSettingsPanel(dawn_engine::DawnEngine *enginePtr) {
        this->selectedShaderStr = enginePtr->getActiveShaderProgram()->getName();
    }

    void HelloWorldUISystem::updateGlobalSettingsPanel(dawn_engine::DawnEngine *enginePtr) {
        ImGui::Begin("Global Settings");
        // shader program selection
        if (ImGui::CollapsingHeader("Shaders", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Button("reload")) {
                enginePtr->getActiveShaderProgram()->reload();
            }
            ImGui::SameLine();
            if (ImGui::BeginCombo("Shader Selector", this->selectedShaderStr.c_str())) {
                for (const auto &mapPair: enginePtr->getShaderProgramMapMeta()) {
                    bool isSelected = selectedShaderStr.c_str() == mapPair.first.c_str();
                    if (ImGui::Selectable(mapPair.first.c_str(), isSelected)) {
                        selectedShaderStr = mapPair.first;
                        enginePtr->setActiveShaderProgram(mapPair.first.c_str());
                    }
                }
                ImGui::EndCombo();
            }

        }
        // main camera properties
        if (ImGui::CollapsingHeader("Main Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            dawn_engine::Camera &camMeta = enginePtr->getMainCameraMeta();
            ImGui::DragFloat("move speed", &camMeta.getMoveSpeedMeta(), this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
            ImGui::DragFloat("sensitivity", &camMeta.getMouseSensitivityMeta(), this->defaultDragSpeed, 0.0f, 1.0f);
            ImGui::DragFloat("field of view", &camMeta.getFovMeta(), this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
            ImGui::DragFloat("zNear", &enginePtr->getMainCameraMeta().getZNearMeta(), this->defaultDragSpeed, 0.001f, enginePtr->getMainCameraMeta().getZFar());
            ImGui::DragFloat("zFar", &enginePtr->getMainCameraMeta().getZFarMeta(), this->defaultDragSpeed, enginePtr->getMainCameraMeta().getZNearMeta(), this->defaultMaxValue);
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

    void HelloWorldUISystem::updateMeshModuleMonitor(dawn_engine::MeshModule *meshModule) {
        if (ImGui::CollapsingHeader("mesh module", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("module activation", &meshModule->getActivationMeta());
            ImGui::Text("mesh num: %zu", meshModule->getMeshes().size());
            std::string label = fmt::format("mesh activation##{}", 0);
            int meshNum = (int) meshModule->getMeshes().size();
            for (int meshIdx = 0; meshIdx < meshNum; ++meshIdx) {
                dawn_engine::DawnMesh &tmpMesh = meshModule->getMeshesMeta()[meshIdx];
                bool enabledLightingMaps = tmpMesh.enableLightingMap();
                std::string treeNodeLabel = fmt::format("mesh node: {}", meshIdx);
                if (ImGui::TreeNode(treeNodeLabel.c_str())) {
                    ImGui::Text("vertex num: %d", meshModule->getMeshesMeta()[meshIdx].verticesNum());
                    std::string materialType = "pure color";
                    if (enabledLightingMaps) {
                        materialType = "lighting map";
                    }
                    std::string materialModeStr = "material mode: " + materialType;
                    ImGui::Text("%s", materialModeStr.c_str());
                    std::string CheckboxLabel = fmt::format("mesh activation##{}", meshIdx);
                    ImGui::Checkbox(CheckboxLabel.c_str(), &meshModule->getActivationsMeta()[meshIdx]);
                    selectedTransparencyTypeStr = meshOpaqueOptions[int(!tmpMesh.getMaterialMeta().getOpaque())];
                    if (ImGui::BeginCombo("Transparency Type", selectedTransparencyTypeStr.c_str())) {
                        for (int i = 0; i < 2; ++i) {
                            bool isSelected = selectedTransparencyTypeStr.c_str() == meshOpaqueOptions[i].c_str();
                            if (ImGui::Selectable(meshOpaqueOptions[i].c_str(), isSelected)) {
                                selectedTransparencyTypeStr = meshOpaqueOptions[i];
                                tmpMesh.getMaterialMeta().setOpaque(!bool(i));
                            }
                        }
                        ImGui::EndCombo();
                    }
                    if (!tmpMesh.getMaterialMeta().getOpaque()) {
                        ImGui::DragFloat("transparency", &tmpMesh.getMaterialMeta().getTransparencyMeta(), this->defaultDragSpeed, 0.0f, 1.0f);
                    }
                    ImGui::ColorEdit3("ambient", &tmpMesh.getMaterialMeta().getAmbientColorMeta()[0]);
                    if (!enabledLightingMaps) {
                        ImGui::ColorEdit3("diffuse", &tmpMesh.getMaterialMeta().getDiffuseColorMeta()[0]);
                        ImGui::ColorEdit3("specular", &tmpMesh.getMaterialMeta().getSpecularColorMeta()[0]);
                    }
                    ImGui::DragFloat("shininess", &tmpMesh.getMaterialMeta().getShininessMeta(), this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
                    ImGui::TreePop();
                }
            }

//            ImGui::ColorEdit3("diffuse", &meshModule->getMaterialMeta().getDiffuseMeta()[0]);
//            ImGui::ColorEdit3("specular", &meshModule->getMaterialMeta().getDiffuseMeta()[0]);
        }
    }

    void HelloWorldUISystem::embedBaseLightMonitor(dawn_engine::LightModule *const lightModule) const {
        ImGui::ColorEdit3("ambient", &lightModule->getAmbientMeta()[0]);
        ImGui::ColorEdit3("diffuse", &lightModule->getDiffuseMeta()[0]);
        ImGui::ColorEdit3("specular", &lightModule->getSpecularMeta()[0]);

    }

    void HelloWorldUISystem::updatePointLightModuleMonitor(dawn_engine::PointLightModule *const pointLightModule) const {
        if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("activation", &pointLightModule->getActivationMeta());
            this->embedBaseLightMonitor(pointLightModule);
            ImGui::Separator();
            ImGui::DragFloat("constant", &pointLightModule->constant, this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
            ImGui::DragFloat("linear", &pointLightModule->linear, this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
            ImGui::DragFloat("quadratic", &pointLightModule->quadratic, this->defaultDragSpeed, 0.0f, this->defaultMaxValue);
        }
    }


    void HelloWorldUISystem::updateDirectionLightModuleMonitor(dawn_engine::DirectionalLightModule *const directionalLightModule) const {
        if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("activation", &directionalLightModule->getActivationMeta());
            this->embedBaseLightMonitor(directionalLightModule);
            ImGui::Separator();
            ImGui::DragFloat3("direction", &directionalLightModule->getDirectionMeta()[0], this->defaultDragSpeed, this->defaultMinValue, this->defaultMaxValue);
        }
    }

    void HelloWorldUISystem::updateSpotLightModuleMonitor(dawn_engine::SpotLightModule *const spotLightModule) const {
        if (ImGui::CollapsingHeader("Spot Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("activation", &spotLightModule->getActivationMeta());
            this->embedBaseLightMonitor(spotLightModule);
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
        } else if (dynamic_cast<dawn_engine::MeshModule *> (targetModule)) {
            this->updateMeshModuleMonitor(dynamic_cast<dawn_engine::MeshModule *> (targetModule));
        } else {
            std::cout << "unknown module" << std::endl;
        }
//        ImGui::End();
    }


}