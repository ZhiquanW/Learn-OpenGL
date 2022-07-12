//
// Created by zhiquan on 5/28/22.
//

#ifndef HELLOWORLD_HELLOWORLD_UI_SYSTEM_H
#define HELLOWORLD_HELLOWORLD_UI_SYSTEM_H

#include "core/dawn_engine.h"
#include "core/dawn_ui_system.h"
#include "render_window.h"

namespace helloworld {

    class HelloWorldUISystem : public dawn_engine::DawnUISystem {
        const std::vector<std::string> meshOpaqueOptions = {"Opaque", "Transparency"};
    private:
        std::vector<int> gameObjectTrackerSelectionTable;
        std::string selected_shader_str;
        std::string selectedTransparencyTypeStr;

        float defaultMinValue = -FLT_MAX;
        float defaultMaxValue = FLT_MAX;
        float defaultDragSpeed = 0.01f;

        void updateGlobalSettingsPanel(dawn_engine::DawnEngine *engine_ptr);

        void initGlobalSettingsPanel(dawn_engine::DawnEngine *enginePtr);

        void initGameObjectTracker(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs);

        void updateGameObjectTracker(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs);

        void initGameObjectMonitors(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs);

        void updateGameObjectMonitors(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs);

        void InitCreationPopup(dawn_engine::DawnEngine *engine_ptr);

        void UpdateCreationPopup(dawn_engine::DawnEngine *engine_ptr);

        void embedModuleMonitor(dawn_engine::BaseModule *targetModule);

        void updateTransformModuleMonitor(dawn_engine::TransformModule *transformModule) const;

        void updateMeshModuleMonitor(dawn_engine::RendererModule *meshModule);

        void EmbedBaseLightMonitor(dawn_engine::LightModule *lightModule) const;

        void updatePointLightModuleMonitor(dawn_engine::PointLightModule *pointLightModule) const;

        void updateDirectionLightModuleMonitor(dawn_engine::DirectionalLightModule *directionalLightModule) const;

        void updateSpotLightModuleMonitor(dawn_engine::SpotLightModule *spotLightModule) const;

        dawn_engine::Ray GenRayOnClick(glm::vec3 start_pos, glm::vec3 cam_dir, float fov, glm::vec2 win_size, glm::vec2 mouse_pos);

        void OnMouseClicked();

    protected:
        void start(dawn_engine::DawnEngine *enginePtr) override;

        void update(dawn_engine::DawnEngine *enginePtr) override;

    public:
        explicit HelloWorldUISystem(int glslVersion);

    };


}

#endif //HELLOWORLD_HELLOWORLD_UI_SYSTEM_H
