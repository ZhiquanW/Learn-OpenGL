//
// Created by zhiquan on 6/9/22.
//

#ifndef HELLOWORLD_DAWN_MATERIAL_H
#define HELLOWORLD_DAWN_MATERIAL_H
# define MAX_BONE_INFLUENCE 4

#include "../../include/common_includes.h"
#include "../../include/shader_uniform_variable.h"

namespace dawn_engine {

    class DawnMaterial {
    private:
    protected:
        glm::vec3 ambientColor = glm::vec3(0.0f);
        glm::vec3 diffuseColor = glm::vec3(1.0f);
        glm::vec3 specularColor = glm::vec3(1.0f);
        float shininess = 3.0f;
        bool opaque = true;
        float transparency = 0.0f;
        std::vector<int> diffuseTexIDs = {};
        std::vector<int> specularTexIDs = {};
        std::vector<int> normalTexIDs = {};

    public:
        DawnMaterial() = default;


        DawnMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

        DawnMaterial(std::vector<int> diffuse, std::vector<int> specular);

        DawnMaterial(std::vector<int> diffuse, std::vector<int> specular, std::vector<int> normal);

        DawnMaterial(glm::vec3 ambient, std::vector<int> diffuse, std::vector<int> specular, float shininess);


        ~DawnMaterial() = default;

        void setOpaque(bool opaque);

        bool getOpaque() const;

        bool &getOpaqueMeta();

        void setTransparency(float alpha);

        [[nodiscard]] float getTransparency() const;

        [[maybe_unused]] float &getTransparencyMeta();

        [[nodiscard]] glm::vec3 getAmbientColor() const;

        [[nodiscard]] glm::vec3 &getAmbientColorMeta();

        void setAmbientColor(glm::vec3 value);

        [[nodiscard]] glm::vec3 getDiffuseColor() const;

        [[nodiscard]] glm::vec3 &getDiffuseColorMeta();

        void setDiffuseColor(glm::vec3 value);

        [[nodiscard]] glm::vec3 getSpecularColor() const;

        [[nodiscard]] glm::vec3 &getSpecularColorMeta();

        void setSpecularColor(glm::vec3 value);

        [[nodiscard]] float getShininess() const;

        [[nodiscard]] float &getShininessMeta();

        void setShininess(float value);

        bool enabledLightingMaps() const;

        [[nodiscard]] std::vector<std::shared_ptr<ShaderUniformVariableBase>> getUniforms() const;

        void bindTextures() const;
    };


}
#endif //HELLOWORLD_DAWN_MATERIAL_H
