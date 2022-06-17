//
// Created by zhiquan on 5/28/22.
//

#ifndef HELLOWORLD_HELLOWORLD_UI_SYSTEM_H
#define HELLOWORLD_HELLOWORLD_UI_SYSTEM_H

#include "core/dawn_ui_system.h"
#include "render_window.h"
#include "core/dawn_engine.h"

namespace helloworld {

    class HelloWorldUISystem : public dawn_engine::DawnUISystem {
    private:
        std::vector<int> gameObjectTrackerSelectionTable;
        std::string selectedShaderStr = "";

        float defaultMinValue = -FLT_MAX;
        float defaultMaxValue = FLT_MAX;
        float defaultDragSpeed = 0.01f;

        void updateGlobalSettingsPanel(dawn_engine::DawnEngine *enginePtr);

        void initGlobalSettingsPanel(dawn_engine::DawnEngine *enginePtr);

        void initGameObjectTracker(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs);

        void updateGameObjectTracker(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs);

        void initGameObjectMonitors(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs);

        void updateGameObjectMonitors(const std::vector<dawn_engine::GameObject *> &gameObjectPtrs);

        void embedModuleMonitor(dawn_engine::BaseModule *targetModule) const;

        void updateTransformModuleMonitor(dawn_engine::TransformModule *transformModule) const;

        void updateMeshModuleMonitor(dawn_engine::MeshModule *meshModule) const;

        void embedBaseLightMonitor(dawn_engine::LightModule *lightModule) const;

        void updatePointLightModuleMonitor(dawn_engine::PointLightModule *pointLightModule) const;

        void updateDirectionLightModuleMonitor(dawn_engine::DirectionalLightModule *directionalLightModule) const;

        void updateSpotLightModuleMonitor(dawn_engine::SpotLightModule *spotLightModule) const;


    protected:
        void start(dawn_engine::DawnEngine *enginePtr) override;

        void update(dawn_engine::DawnEngine *enginePtr) override;

    public:
        explicit HelloWorldUISystem(int glslVersion);

    };


}

#endif //HELLOWORLD_HELLOWORLD_UI_SYSTEM_H
