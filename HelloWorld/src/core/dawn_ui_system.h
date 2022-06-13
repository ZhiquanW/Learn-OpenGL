#pragma once

#include "common_includes.h"

namespace dawn_engine {
    class DawnEngine;

    class DawnUISystem {
    private:
        int glslVersion;

    protected:
        void preUpdate();

        void postUpdate();


        virtual void update(DawnEngine *enginePtr);

    public:

        explicit DawnUISystem(int glslVersion);

        ~DawnUISystem();

        void initialize(GLFWwindow *glfwWindow);

        virtual void start(DawnEngine *enginePtr);

        void render(DawnEngine *enginePtr);

        [[nodiscard]] DawnEngine *getEnginePtr() const;

        void setEnginePtr(DawnEngine *dawnEnginePtr) const;

    };

} // namespace dawn_engine