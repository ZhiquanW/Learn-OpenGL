#pragma once

#include "dawn_engine.h"
#include "helloworld_ui_system.h"

namespace helloworld {
    class HelloWorldApp : public dawn_engine::DawnEngine {
    private:
        void addDefaultLight();

        void addDefaultCube();

    public:
        HelloWorldApp(uint width, uint height);

        void awake() override;

        void start() override;

        void update() override;
    };
} // namespace helloworld