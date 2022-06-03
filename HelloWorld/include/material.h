#pragma once

#include "common_includes.h"
#include "shader_uniform_variable.h"

namespace dawn_engine {
    class Material {
    protected:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;

    public:
        Material();

        Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

        [[nodiscard]] glm::vec3 getAmbient() const;

        [[nodiscard]] glm::vec3 &getAmbientMeta();

        void setAmbient(glm::vec3 value);

        [[nodiscard]] glm::vec3 getDiffuse() const;

        [[nodiscard]] glm::vec3 &getDiffuseMeta();

        void setDiffuse(glm::vec3 value);

        [[nodiscard]] glm::vec3 getSpecular() const;

        [[nodiscard]] glm::vec3 &getSpecularMeta();

        void setSpecular(glm::vec3 value);

        [[nodiscard]] float getShininess() const;

        [[nodiscard]] float &getShininessMeta();

        void setShinibess(float value);

        std::vector<std::shared_ptr<ShaderUniformVariableBase>> getUniforms(const std::string &name) const;
    };
} // namespace dawn_engine