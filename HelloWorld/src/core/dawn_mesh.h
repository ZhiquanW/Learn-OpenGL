#pragma once


#include "../modules/base_module.h"
#include "graphics_layer/gl_shader_program.h"
#include "dawn_material.h"
#include "dawn_vertex.h"

namespace dawn_engine {
    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);


    class DawnMesh {
    private:

    protected:
        std::vector<DawnVertex> vertices_;
        std::vector<unsigned int> indices_;
        DawnMaterial material_;

    public:
        DawnMesh();

        DawnMesh(const std::vector<DawnVertex> &vertices, const std::vector<unsigned int> &indices, const DawnMaterial &material);

        ~DawnMesh() = default;

//        void render(GLShaderProgram *shaderProgram) const;

        [[nodiscard]] unsigned int GetVerticesNum() const;

        unsigned int GetIndicesNum() const;

        bool EnableLightingMap() const;

        DawnMaterial GetMaterial() const;

        DawnMaterial &GetMaterialRef();

        std::vector<DawnVertex> &GetVerticesRef();

        std::vector<unsigned int> &GetIndicesRef();
    };

} // namespace dawn_engine