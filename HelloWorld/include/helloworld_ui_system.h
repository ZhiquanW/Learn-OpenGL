//
// Created by zhiquan on 5/28/22.
//

#ifndef HELLOWORLD_HELLOWORLD_UI_SYSTEM_H
#define HELLOWORLD_HELLOWORLD_UI_SYSTEM_H

#include "core/dawn_engine.h"
#include "core/dawn_ui_system.h"
#include "render_window.h"
#include "behaviour_modules/box_block.h"

namespace helloworld {

    class HelloWorldUISystem : public dawn_engine::DawnUISystem {
        const std::vector<std::string> meshOpaqueOptions = {"Opaque", "Transparency"};
    private:
        std::vector<int> gameObjectTrackerSelectionTable;
        std::vector<int> prefab_selection_table{};
        std::string selected_shader_str;
        std::string selectedTransparencyTypeStr;

        float defaultMinValue = -FLT_MAX;
        float defaultMaxValue = FLT_MAX;
        float defaultDragSpeed = 0.01f;

        void InitGlobalSettingsPanel(dawn_engine::DawnEngine *enginePtr);

        void UpdateGlobalSettingsPanel(dawn_engine::DawnEngine *engine_ptr);

        void InitPrefabManager();

        void UpdatePrefabManager(const std::set<std::string> &prefab_paths);

        void InitGameObjectTracker(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs);

        void UpdateGameObjectTracker(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs);

        void InitGameObjectMonitors(const std::vector<dawn_engine::GameObject *> &game_obj_ptrs);

        void UpdateGameObjectMonitors(const std::vector<dawn_engine::GameObject *> &game_objs);

        void UpdateCreationPopup(dawn_engine::DawnEngine *engine_ptr);

        void EmbedModuleMonitor(dawn_engine::BaseModule *targetModule);

        void UpdateBehaviourModule(dawn_engine::BehaviourModule *behaviour_module);

        void UpdateTransformModuleMonitor(dawn_engine::TransformModule *const transformModule) const;

        void UpdateMeshModuleMonitor(dawn_engine::RendererModule *meshModule);

        void EmbedBaseLightMonitor(dawn_engine::LightModule *lightModule) const;

        void UpdatePointLightModuleMonitor(dawn_engine::PointLightModule *const pointLightModule) const;

        void UpdateDirectionLightModuleMonitor(dawn_engine::DirectionalLightModule *const directionalLightModule) const;

        void UpdateSpotLightModuleMonitor(dawn_engine::SpotLightModule *const spotLightModule) const;

        void UpdateColliderModuleMonitor(dawn_engine::ColliderModule *const collider_module) const;

        Ray GenRayOnClick(glm::vec3 start_pos, glm::vec3 cam_front, glm::vec3 cam_up, glm::vec3 cam_right, float fov,
                          glm::vec2 win_size, glm::vec2 mouse_pos);

        void OnMouseClicked();

    protected:
        void start(dawn_engine::DawnEngine *enginePtr) override;

        void update(dawn_engine::DawnEngine *enginePtr) override;

    public:
        explicit HelloWorldUISystem(int glslVersion);

    };


}

#endif //HELLOWORLD_HELLOWORLD_UI_SYSTEM_H
