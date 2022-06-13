#pragma once

#include "base_module.h"
#include "../core/dawn_mesh.h"

namespace dawn_engine {

    class MeshModule : public BaseModule {
    private:

    protected:
        std::vector<DawnMesh> meshes;
        std::deque<bool> activations;

    public:
        static const std::size_t type;

        MeshModule();

        MeshModule(std::vector<DawnMesh> meshes);

        ~MeshModule();

        void setAsRenderTarget() const;

        [[nodiscard]] std::vector<DawnMesh> getMeshes() const;


        [[nodiscard]] std::vector<DawnMesh> &getMeshesMeta();

        [[nodiscard]] std::deque<bool> getActivations() const;

        void activateMesh(int idx);

        void deactivateMesh(int idx);

        [[nodiscard]] std::deque<bool> &getActivationsMeta();

        void render(OpenGLShaderProgram *shaderProgram) const;
    };

} // namespace dawn_engine