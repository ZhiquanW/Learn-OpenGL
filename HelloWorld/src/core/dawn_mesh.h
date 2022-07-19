#pragma once


#include "../modules/base_module.h"
#include "graphics_layer/gl_shader_program.h"
#include "dawn_material.h"
#include "dawn_vertex.h"

namespace dawn_engine {


    class DawnMesh {
    private:

    protected:
        std::vector<DawnVertex> vertices_;
        std::vector<unsigned int> indices_;
        std::shared_ptr<DawnMaterial> material_ptr_;

    public:
        DawnMesh();

        DawnMesh(const std::vector<DawnVertex> &vertices, const std::vector<unsigned int> &indices, const std::shared_ptr<DawnMaterial> & material_ptr);

        ~DawnMesh() = default;

//        void render(GLShaderProgram *shaderProgram) const;

        [[nodiscard]] unsigned int GetVerticesNum() const;

        unsigned int GetIndicesNum() const;

        bool EnableLightingMap() const;

        std::shared_ptr<DawnMaterial> GetMaterialPtr() const;

        void SetMaterialPtr(const std::shared_ptr<DawnMaterial> &material_ptr);

        std::vector<DawnVertex> &GetVerticesRef();

        std::vector<unsigned int> &GetIndicesRef();
    };

} // namespace dawn_engine