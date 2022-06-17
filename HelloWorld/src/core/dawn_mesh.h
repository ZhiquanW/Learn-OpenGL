#pragma once


#include "../modules/base_module.h"
#include "../../include/opengl_shader_program.h"
#include "dawn_material.h"

namespace dawn_engine {
    int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

# define MAX_BONE_INFLUENCE 4

    struct Vertex {
        glm::vec3 position{};
        glm::vec3 normal{};
        glm::vec2 texCoords{};
        glm::vec3 tangent{};
        glm::vec3 biTangent{};
        int mBoneIDs[MAX_BONE_INFLUENCE] = {};
        float mWeights[MAX_BONE_INFLUENCE] = {};

//        Vertex(float x, float y, float z, float normalX, float normalY, float normalZ, float texX, float texY) {
//
//        }


    };


    class DawnMesh {
    private:

        unsigned int glVAO{}, glVBO{}, glEBO{};


    protected:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        DawnMaterial material;

    public:
        DawnMesh();

        DawnMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const DawnMaterial &material);

        ~DawnMesh();

        void initGLData();

        void deleteGLData();

        void render(OpenGLShaderProgram *shaderProgram) const;

        [[nodiscard]] unsigned int verticesNum() const;

        bool enableLightingMap() const;

        DawnMaterial getMaterial() const;

        DawnMaterial &getMaterialMeta();
    };

} // namespace dawn_engine