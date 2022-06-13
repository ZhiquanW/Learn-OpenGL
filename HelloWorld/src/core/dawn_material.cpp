//
// Created by zhiquan on 6/9/22.
//

#include "dawn_material.h"

#include <utility>

namespace dawn_engine {


    DawnMaterial::DawnMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
            : ambientColor(ambient), diffuseColor(diffuse), specularColor(specular), shininess(shininess) {
    }

    DawnMaterial::DawnMaterial(std::vector<int> diffuse, std::vector<int> specular) : diffuseTexIDs(std::move(diffuse)), specularTexIDs(std::move(specular)) {
    }

    DawnMaterial::DawnMaterial(glm::vec3 ambient, std::vector<int> diffuse, std::vector<int> specular, float shininess)
            : ambientColor(ambient), diffuseTexIDs(std::move(diffuse)), specularTexIDs(std::move(specular)), shininess(shininess) {
    }

    glm::vec3 DawnMaterial::getAmbientColor() const {
        return this->ambientColor;
    }

    glm::vec3 &DawnMaterial::getAmbientColorMeta() {
        return this->ambientColor;
    }

    void DawnMaterial::setAmbientColor(glm::vec3 value) {
        this->ambientColor = value;

    }

    glm::vec3 DawnMaterial::getDiffuseColor() const {
        return this->diffuseColor;
    }

    glm::vec3 &DawnMaterial::getDiffuseColorMeta() {
        return this->diffuseColor;
    }

    void DawnMaterial::setDiffuseColor(glm::vec3 value) {
        this->diffuseColor = value;

    }

    glm::vec3 DawnMaterial::getSpecularColor() const {
        return this->specularColor;
    }

    glm::vec3 &DawnMaterial::getSpecularColorMeta() {
        return this->specularColor;
    }

    void DawnMaterial::setSpecularColor(glm::vec3 value) {
        this->specularColor = value;

    }

    float DawnMaterial::getShininess() const {
        return this->shininess;
    }

    float &DawnMaterial::getShininessMeta() {
        return this->shininess;
    }

    void DawnMaterial::setShininess(float value) {
        this->shininess = value;
    }

    bool DawnMaterial::enabledLightingMaps() const {
        return !this->diffuseTexIDs.empty();
    }

    std::vector<std::shared_ptr<ShaderUniformVariableBase>> DawnMaterial::getUniforms() const {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms;
        uniforms.emplace_back(std::make_shared<ShaderUniformVariable<bool>>("material.enable_lighting_maps", this->enabledLightingMaps()));
        uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>("material.ambient", this->ambientColor));
        uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>("material.diffuse", this->diffuseColor));
        uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>("material.specular", this->specularColor));
        uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>("material.shininess", this->shininess));
        unsigned int textureUnitIdx = 0;
        for (auto texID: this->diffuseTexIDs) {
            std::string uniformName = fmt::format("material.diffuse_texture_{}", textureUnitIdx);
            uniforms.emplace_back(std::make_shared<ShaderUniformVariable<int>>(uniformName, textureUnitIdx++));
        }
        for (auto texID: this->diffuseTexIDs) {
            std::string uniformName = fmt::format("material.specular_texture_{}", textureUnitIdx);
            uniforms.emplace_back(std::make_shared<ShaderUniformVariable<int>>(uniformName, textureUnitIdx++));
        }
        return uniforms;
    }

    void DawnMaterial::bindTextures() const {
        int textureUnitIdx = 0;
        for (auto texID: this->diffuseTexIDs) {
            glActiveTexture(GL_TEXTURE0 + textureUnitIdx++);
            glBindTexture(GL_TEXTURE_2D, texID);
        }
        for (auto texID: this->specularTexIDs) {
            glActiveTexture(GL_TEXTURE0 + textureUnitIdx++);
            glBindTexture(GL_TEXTURE_2D, texID);
        }

    }


}
